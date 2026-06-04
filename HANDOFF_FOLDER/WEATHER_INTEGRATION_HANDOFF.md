# Weather Integration Handoff
**Date:** June 3, 2026 | **Status:** Foundation complete, integration layers needed

---

## What's Already Built (do not touch)

### `main/weather_icons.h` + `main/weather_icons.c`
Complete WMO code → icon lookup. Returns `lv_image_dsc_t*` pointers to embedded images.

```c
// Usage:
lv_image_set_src(widget, weather_icon_60(wmo_code, is_night));  // 60x60 main icon
lv_image_set_src(widget, weather_icon_22(wmo_code, is_night));  // 22x22 hourly icon
const char *desc = weather_description(wmo_code);               // "Partly Cloudy" etc.
```

### `main/weather_display.h` + `main/weather_display.c`
All 36 home screen widgets wired up. Fill a `weather_data_t` struct and call one function.

```c
weather_data_t w = { ... };
bsp_display_lock(-1);
weather_display_update(&w);
bsp_display_unlock();
```

Also: `weather_display_update_time(str)` and `weather_display_update_network(bool connected)`.

---

## Correct Widget Names (plan doc had errors)

| Plan Doc Name | Actual SquareLine Name |
|---------------|----------------------|
| `HUMIDITYQ` | `GUI_Label__home__HUMIDITYPERCENTQ` |
| `RAINPERCENT_Q` | `GUI_Label__home__CHANCEOFRAINQ` |
| `WINDQ` (Label) | `GUI_Container__home__CURRENTWINDQ` (child label at index 0) |
| `HOURnTIMEQ` | `GUI_Label__home__HRnFORCASTTIMEQ` |
| `HOURnICONQ` | `GUI_Image__home__HRnFORCASICON22X22Q` |
| `HOURnTEMPQ` | `GUI_Label__home__HRnFORCASTTEMPQ` |
| `TODAYHIGHQ/LOWQ` | **NOT IN UI — skip** |
| Hourly rain % | **NOT IN UI — skip** |
| *(new)* | `GUI_Image__home__NETWORKCONNECTEDICONQ` (green/red WiFi icon) |

---

## What Haiku Needs to Build

### 1. SquareLine UI Integration into CMakeLists
The SquareLine export is at:
```
UI/DESK MEDIA DEVICEV2 UI_UIFiles/
```
All `.c` files from that folder need to be compiled. The `GUI.h` file is the main header.

**Critical:** UI init must happen AFTER `bsp_display_start_with_config()` and INSIDE `bsp_display_lock()`. See `BOARD_TIPS_AND_QUIRKS.md` — display init order is sensitive.

Add to `main/CMakeLists.txt`: all SquareLine .c files as sources, include paths for the UI folder.

### 2. WiFi Setup (board-specific — ESP32-C6 via SDIO)
This board has NO built-in WiFi on the ESP32-P4. WiFi goes through an ESP32-C6 module via SDIO.

Add these components (same as Waveshare example 04_wifistation):
```
idf.py add-dependency espressif/esp_wifi_remote==0.14.*
idf.py add-dependency espressif/esp_hosted==1.4.*
```

After adding: use `esp_wifi` API normally — the components handle the SDIO bridge transparently.

WiFi credentials come from NVS (ZIP code screen → settings screen stores SSID/password).

### 3. HTTP Fetch + JSON Parse (Open-Meteo)
Use ESP-IDF's built-in `esp_http_client` and `cJSON`.

API endpoint (no API key needed):
```
https://api.open-meteo.com/v1/forecast
?latitude=LAT&longitude=LON
&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,
         precipitation_probability,wind_speed_10m
&hourly=temperature_2m,weather_code
&daily=temperature_2m_max,temperature_2m_min,weather_code
&temperature_unit=fahrenheit
&wind_speed_unit=mph
&timezone=auto
&forecast_days=4
&forecast_hours=6
```

Parse into `weather_data_t` (defined in `weather_display.h`).

**WMO codes Open-Meteo actually returns:** 0,1,2,3,45,48,51-57,61-67,71-77,80-82,85-86,95-99
**Codes 611-616 from the old plan doc do NOT exist — ignore them.**

### 4. ZIP Code → Coordinates (Geocoding)
When user enters ZIP on `keyboardzipcode` screen, call a geocoding API to get lat/lon/city/state.
Options (free, no key needed): `nominatim.openstreetmap.org`

```
GET https://nominatim.openstreetmap.org/search?postalcode=80202&country=US&format=json
```

Returns lat, lon, display_name. Parse city/state from display_name or use structured fields.

Store in NVS: `lat`, `lon`, `city`, `state`.

### 5. FreeRTOS Weather Update Task
```c
void weather_task(void *param) {
    while (1) {
        if (wifi_connected) {
            fetch_and_parse_weather();   // fills weather_data_t
            bsp_display_lock(-1);
            weather_display_update(&weather_data);
            bsp_display_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(15 * 60 * 1000));  // 15 minutes
    }
}
```

Start this task AFTER display and WiFi are initialized.

### 6. NVS Storage
Store and retrieve:
- `zip` — ZIP code string
- `lat`, `lon` — float as string
- `city`, `state` — location strings
- `ssid`, `password` — WiFi credentials

---

## Files to NOT Change
- `main/weather_icons.h` — icon declarations (all 30 icons mapped)
- `main/weather_icons.c` — WMO lookup tables
- `main/weather_display.h` — `weather_data_t` struct + function signatures
- `main/weather_display.c` — widget update implementation

---

## Key References
- WiFi setup: `docs/ESP32-P4-WIFI6-Touch-LCD-4.3-main/examples/esp-idf/04_wifistation/`
- Display init rules: `HANDOFF_FOLDER/BOARD_TIPS_AND_QUIRKS.md`
- SquareLine screens: `UI/DESK MEDIA DEVICEV2 UI_UIFiles/screens/`
- Board quirks / build rules: `docs/behaviors and rules.md`
