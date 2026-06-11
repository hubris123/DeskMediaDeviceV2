# Weather Proxy Cache — Design Plan

## What It Is

A lightweight middleman server that sits between ESP32 devices and the
weather API. Devices never call the real API directly — they call the proxy,
which returns cached data if it's fresh, or fetches new data if it's stale.

## Why Build It

- Multiple devices requesting the same ZIP code hit the API once instead of
  once per device per refresh cycle
- Open-Meteo is free but rate-limited — the proxy keeps usage well below limits
- Devices get faster responses (local/hosted cache vs. external API round trip)
- If the weather API goes down, devices still receive the last known good data
- One central place to swap API providers without touching firmware

---

## How It Operates

1. ESP32 sends a GET request to the proxy:
   `https://yoursite.com/weather.php?zip=89031`

2. Proxy checks its cache (MySQL row or flat JSON file) for that ZIP:
   - **Cache hit, data fresh (under 20 min):** return cached JSON immediately,
     no API call made
   - **Cache miss or stale:** call the real Open-Meteo geocoding + weather
     endpoints, store the result with a timestamp, return it to the device

3. ESP32 receives the same JSON structure it currently expects — no firmware
   changes needed beyond swapping the URL

---

## Hosting — GoDaddy Shared Hosting (PHP + MySQL)

GoDaddy shared hosting is a good fit because:
- Always on, no local hardware to maintain
- HTTPS included — ESP32 already validates certs so this is required
- Works from any network (not limited to home WiFi)
- MySQL database usually included in the plan
- PHP outbound HTTP requests (via cURL) allowed on most GoDaddy plans

**Verify before building:** confirm your GoDaddy plan allows outbound cURL
requests from PHP. Log into cPanel → PHP info and check `curl` is enabled.

---

## File / Database Structure

### MySQL table (preferred over flat files — faster, concurrent-safe)
```sql
CREATE TABLE weather_cache (
    zip         VARCHAR(10) PRIMARY KEY,
    lat         FLOAT,
    lon         FLOAT,
    city        VARCHAR(64),
    state       VARCHAR(8),
    data_json   TEXT,
    updated_at  INT   -- Unix timestamp
);
```

### Flat file fallback (if no MySQL)
One JSON file per ZIP: `cache/89031.json`
Contains the full response + a `cached_at` Unix timestamp field.

---

## PHP Proxy Script (weather.php) — Logic Overview

```
1. Read ?zip= from query string, sanitize to digits only
2. Connect to MySQL (or read cache file)
3. Check if row exists AND (now - updated_at) < 1200 seconds (20 min)
   YES → echo cached data_json, exit
   NO  → continue
4. Call Open-Meteo geocoding: api.zippopotam.us/us/{zip}
   Parse lat, lon, city, state
5. Call Open-Meteo weather: api.open-meteo.com/v1/forecast?...
   Parse temp, precip, weather code, hourly, daily
6. Build response JSON in the same format the ESP32 already expects
7. Store in MySQL with updated_at = time()
8. Echo response JSON
```

Total script length: ~120 lines of PHP including error handling.

---

## ESP32 Firmware Change (minimal)

In `main/weather/weather_api.c`, change two URL constants:

```c
// Before
#define GEOCODING_URL  "http://api.zippopotam.us/us/%s"
#define WEATHER_URL    "https://api.open-meteo.com/v1/forecast?..."

// After
#define PROXY_URL  "https://yoursite.com/weather.php?zip=%s"
```

The proxy returns the same JSON the ESP32 already parses, so `weather_api.c`
parse logic is unchanged. Only the URL and the single HTTP call change.

---

## Cache Lifetime Recommendation

| Data type       | Suggested TTL |
|-----------------|---------------|
| Weather data    | 20 minutes    |
| Geocoded coords | 24 hours (ZIP→lat/lon never changes) |

Store geocoding separately so a stale weather cache doesn't re-geocode.

---

## Future Expansion

- Add an `/api/firmware` endpoint on the same server for OTA manifest hosting
- Add a simple admin page to view cache hits/misses and force-expire a ZIP
- Support multiple devices in different cities automatically — the cache
  handles any ZIP, not just one hardcoded location
- Rate-limit by IP if the proxy is ever made public

---

## Build Order (when ready)

1. Verify GoDaddy plan has cURL + MySQL
2. Create MySQL table via cPanel phpMyAdmin
3. Write and deploy `weather.php`
4. Test with a browser: `https://yoursite.com/weather.php?zip=89031`
5. Update `weather_api.c` URL, rebuild firmware, flash, verify
6. Monitor proxy logs for a day to confirm cache hit rate
