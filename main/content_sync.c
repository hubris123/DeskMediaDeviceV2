#include "content_sync.h"
#include "wifi_manager.h"
#include "power_gate.h"

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"

static const char *TAG = "ContentSync";

#define CONTENT_RELEASE_URL  "https://api.github.com/repos/hubris123/DeskMediaDeviceV2/releases/tags/content"
#define CONTENT_USER_AGENT   "DeskMediaDevice/1.0"
#define MANIFEST_ASSET_NAME  "content_manifest.json"

#define MUSIC_DIR  "/sdcard/music"
#define VIDEO_DIR  "/sdcard/video"

#define FIRST_SYNC_DELAY_MS   (75 * 1000)            // after OTA's first check, stagger ours
#define RECHECK_PERIOD_MS     (24 * 60 * 60 * 1000)  // daily
#define RELEASE_JSON_CAP      (24 * 1024)            // GitHub release API response
#define MANIFEST_CAP          (8 * 1024)             // content_manifest.json

// Embedded fallback manifest (used only if the live one can't be fetched/parsed).
extern const char default_content_manifest_start[] asm("_binary_default_content_manifest_json_start");
extern const char default_content_manifest_end[]   asm("_binary_default_content_manifest_json_end");

static volatile bool s_sync_running = false;

// ── HTTP download (follows GitHub's redirect to the asset CDN) ────────────────

typedef struct {
    FILE   *f;        // file sink (download_to_file), or
    char   *mem;      // memory sink (download_to_mem)
    size_t  cap;
    size_t  len;
    bool    overflow;
} dl_ctx_t;

static esp_err_t dl_event_cb(esp_http_client_event_t *evt)
{
    if (evt->event_id != HTTP_EVENT_ON_DATA) return ESP_OK;
    // During a 3xx redirect the body is a throwaway page - only capture the
    // final 200 response so we never write redirect HTML into the file/buffer.
    if (esp_http_client_get_status_code(evt->client) != 200) return ESP_OK;

    dl_ctx_t *c = (dl_ctx_t *)evt->user_data;
    if (!c) return ESP_OK;
    if (c->f) {
        fwrite(evt->data, 1, evt->data_len, c->f);
    } else if (c->mem) {
        if (c->len + (size_t)evt->data_len < c->cap) {
            memcpy(c->mem + c->len, evt->data, evt->data_len);
            c->len += evt->data_len;
        } else {
            c->overflow = true;
        }
    }
    return ESP_OK;
}

// GET url into a NUL-terminated memory buffer. accept_header may be NULL.
static esp_err_t http_get_mem(const char *url, const char *accept_header,
                              char *buf, size_t cap, size_t *out_len)
{
    dl_ctx_t ctx = { .mem = buf, .cap = cap };
    esp_http_client_config_t cfg = {
        .url = url,
        .user_agent = CONTENT_USER_AGENT,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 20000,
        .buffer_size = 4096,
        .buffer_size_tx = 2048,   // GitHub redirect URLs are long
        .event_handler = dl_event_cb,
        .user_data = &ctx,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) return ESP_FAIL;
    if (accept_header) esp_http_client_set_header(client, "Accept", accept_header);

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK) {
        ESP_LOGW(TAG, "GET failed: %s", esp_err_to_name(err));
        return err;
    }
    if (status != 200) {
        ESP_LOGW(TAG, "GET HTTP %d for %s", status, url);
        return ESP_FAIL;
    }
    if (ctx.overflow) {
        ESP_LOGW(TAG, "Response exceeded %u byte buffer", (unsigned)cap);
        return ESP_FAIL;
    }
    if (ctx.len >= cap) return ESP_FAIL;
    buf[ctx.len] = '\0';
    if (out_len) *out_len = ctx.len;
    return ESP_OK;
}

// GET url straight to dest_path (atomically, via a .tmp then rename).
static esp_err_t http_get_file(const char *url, const char *dest_path)
{
    char tmp_path[300];
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", dest_path);

    FILE *f = fopen(tmp_path, "wb");
    if (!f) {
        ESP_LOGE(TAG, "Cannot open %s for write", tmp_path);
        return ESP_FAIL;
    }

    dl_ctx_t ctx = { .f = f };
    esp_http_client_config_t cfg = {
        .url = url,
        .user_agent = CONTENT_USER_AGENT,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 30000,
        .buffer_size = 8192,
        .buffer_size_tx = 2048,
        .event_handler = dl_event_cb,
        .user_data = &ctx,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) { fclose(f); remove(tmp_path); return ESP_FAIL; }

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);
    fclose(f);

    if (err != ESP_OK || status != 200) {
        ESP_LOGW(TAG, "Download failed (%s, HTTP %d): %s",
                 esp_err_to_name(err), status, url);
        remove(tmp_path);
        return ESP_FAIL;
    }

    // Atomic swap: FATFS rename won't overwrite, so drop any existing first.
    remove(dest_path);
    if (rename(tmp_path, dest_path) != 0) {
        ESP_LOGE(TAG, "rename %s -> %s failed", tmp_path, dest_path);
        remove(tmp_path);
        return ESP_FAIL;
    }
    return ESP_OK;
}

// ── Manifest + release asset matching ────────────────────────────────────────

// Find an asset by name in the release JSON's "assets" array; returns its
// browser_download_url and size, or false if not present.
static bool find_asset(cJSON *release, const char *name,
                       const char **url_out, long *size_out)
{
    cJSON *assets = cJSON_GetObjectItem(release, "assets");
    if (!cJSON_IsArray(assets)) return false;
    cJSON *a;
    cJSON_ArrayForEach(a, assets) {
        cJSON *n  = cJSON_GetObjectItem(a, "name");
        cJSON *dl = cJSON_GetObjectItem(a, "browser_download_url");
        if (cJSON_IsString(n) && cJSON_IsString(dl) &&
            strcmp(n->valuestring, name) == 0) {
            *url_out = dl->valuestring;
            cJSON *sz = cJSON_GetObjectItem(a, "size");
            *size_out = cJSON_IsNumber(sz) ? (long)sz->valuedouble : -1;
            return true;
        }
    }
    return false;
}

// Map a manifest "dest" to its SD folder. Returns NULL for unsupported dests.
static const char *dest_dir(const char *dest)
{
    if (strcmp(dest, "music") == 0) return MUSIC_DIR;
    if (strcmp(dest, "video") == 0) return VIDEO_DIR;
    return NULL;   // "background" handled in phase 2; unknown -> skip
}

// Process one manifest item: download if missing or size-mismatched.
static void sync_item(cJSON *release, const char *file, const char *dest,
                      int *downloaded, int *skipped)
{
    const char *dir = dest_dir(dest);
    if (!dir) {
        ESP_LOGI(TAG, "Skipping '%s' (dest '%s' not handled yet)", file, dest);
        return;
    }

    const char *url = NULL;
    long asset_size = -1;
    if (!find_asset(release, file, &url, &asset_size)) {
        ESP_LOGW(TAG, "Manifest lists '%s' but no such asset in the release", file);
        return;
    }

    char path[300];
    snprintf(path, sizeof(path), "%s/%s", dir, file);

    struct stat st;
    if (stat(path, &st) == 0 && (asset_size < 0 || st.st_size == asset_size)) {
        (*skipped)++;
        return;   // already present and the right size
    }

    ESP_LOGI(TAG, "Downloading %s (%ld bytes) -> %s", file, asset_size, path);
    if (http_get_file(url, path) == ESP_OK) {
        ESP_LOGI(TAG, "  done: %s", path);
        (*downloaded)++;
    }
}

// Walk a parsed manifest's items[] and sync each one.
static void process_manifest(cJSON *release, cJSON *manifest)
{
    cJSON *version = cJSON_GetObjectItem(manifest, "version");
    if (cJSON_IsNumber(version)) {
        ESP_LOGI(TAG, "Manifest version %d", (int)version->valuedouble);
    }
    cJSON *items = cJSON_GetObjectItem(manifest, "items");
    if (!cJSON_IsArray(items)) {
        ESP_LOGW(TAG, "Manifest has no items[] array");
        return;
    }

    int downloaded = 0, skipped = 0;
    cJSON *it;
    cJSON_ArrayForEach(it, items) {
        cJSON *file = cJSON_GetObjectItem(it, "file");
        cJSON *dest = cJSON_GetObjectItem(it, "dest");
        if (!cJSON_IsString(file) || !cJSON_IsString(dest)) continue;
        sync_item(release, file->valuestring, dest->valuestring,
                  &downloaded, &skipped);
    }
    ESP_LOGI(TAG, "Sync complete: %d downloaded, %d already present", downloaded, skipped);
    if (downloaded > 0) {
        ESP_LOGI(TAG, "New content will load on the next reboot");
    }
}

// Make sure the destination folders exist (mkdir is a no-op if they do).
static void ensure_dirs(void)
{
    mkdir(MUSIC_DIR, 0777);
    mkdir(VIDEO_DIR, 0777);
}

// ── One full sync pass ───────────────────────────────────────────────────────

static void do_sync(void)
{
    ESP_LOGI(TAG, "Checking content release...");

    char *release_buf = malloc(RELEASE_JSON_CAP);
    if (!release_buf) return;

    size_t rlen = 0;
    if (http_get_mem(CONTENT_RELEASE_URL, "application/vnd.github+json",
                     release_buf, RELEASE_JSON_CAP, &rlen) != ESP_OK) {
        ESP_LOGW(TAG, "No content release found (or network) - nothing to sync");
        free(release_buf);
        return;
    }

    cJSON *release = cJSON_Parse(release_buf);
    free(release_buf);
    if (!release) {
        ESP_LOGW(TAG, "Could not parse release JSON");
        return;
    }

    ensure_dirs();

    // Prefer the live manifest asset; fall back to the embedded default.
    cJSON *manifest = NULL;
    const char *murl = NULL;
    long msize = -1;
    if (find_asset(release, MANIFEST_ASSET_NAME, &murl, &msize)) {
        char *mbuf = malloc(MANIFEST_CAP);
        if (mbuf) {
            size_t mlen = 0;
            if (http_get_mem(murl, NULL, mbuf, MANIFEST_CAP, &mlen) == ESP_OK) {
                manifest = cJSON_Parse(mbuf);
                if (!manifest) ESP_LOGW(TAG, "Live manifest parse failed");
            }
            free(mbuf);
        }
    } else {
        ESP_LOGW(TAG, "Release has no %s asset", MANIFEST_ASSET_NAME);
    }

    if (!manifest) {
        ESP_LOGW(TAG, "Falling back to embedded default manifest");
        manifest = cJSON_Parse(default_content_manifest_start);
    }

    if (manifest) {
        process_manifest(release, manifest);
        cJSON_Delete(manifest);
    } else {
        ESP_LOGE(TAG, "No usable manifest");
    }

    cJSON_Delete(release);
}

// ── Tasks ────────────────────────────────────────────────────────────────────

static void sync_once_task(void *arg)
{
    (void)arg;
    if (s_sync_running) { vTaskDelete(NULL); return; }
    s_sync_running = true;

    // Wait until WiFi is actually up (up to ~30s) before hitting the network.
    for (int i = 0; i < 60 && !wifi_manager_is_connected(); i++) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    if (wifi_manager_is_connected()) {
        // Power interlock: wait for a quiet, video-free, idle moment before any HTTP.
        power_gate_net_wait_and_begin();
        do_sync();
        power_gate_net_end();
    } else {
        ESP_LOGW(TAG, "WiFi not connected - skipping content sync");
    }

    s_sync_running = false;
    vTaskDelete(NULL);
}

static void content_check_task(void *arg)
{
    (void)arg;
    vTaskDelay(pdMS_TO_TICKS(FIRST_SYNC_DELAY_MS));
    for (;;) {
        if (!s_sync_running) {
            xTaskCreate(sync_once_task, "content_sync", 8192, NULL, 3, NULL);
        }
        vTaskDelay(pdMS_TO_TICKS(RECHECK_PERIOD_MS));
    }
}

// ── Public API ───────────────────────────────────────────────────────────────

void content_sync_start(void)
{
    xTaskCreate(content_check_task, "content_check", 4096, NULL, 3, NULL);
}

void content_sync_now(void)
{
    if (s_sync_running) {
        ESP_LOGW(TAG, "Sync already running");
        return;
    }
    xTaskCreate(sync_once_task, "content_sync", 8192, NULL, 3, NULL);
}
