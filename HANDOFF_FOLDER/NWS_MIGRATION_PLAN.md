# NWS Migration Plan — Replace Open-Meteo Completely
**Created:** June 10, 2026 (late night) | **Decision:** User confirmed full move to api.weather.gov
**Why:** Open-Meteo HRRR current temp measured 12°F low vs reality (81.9 vs 94 at KVGT).
Forecasts presumed similarly biased. NWS serves actual station observations + official
NWS forecasts — same upstream data AccuWeather displays. Free, no key, public domain.

---

## API Rules
- No key. REQUIRED: User-Agent with app + contact:
  `DeskMediaDevice/1.0 (mvavrick@gmail.com)`
- Rate limit unpublished, ~5000/hr per source IP. Our usage: ~6 req/hr. Nothing.
- HTTPS only (existing crt_bundle works). All JSON (GeoJSON-ish).

## Endpoint Map

### 0. Zip → lat/lon/city/state — KEEP zippopotam.us (NWS has no zip lookup)

### 1. One-time per zip: /points/{lat},{lon}
`https://api.weather.gov/points/36.2589,-115.1718`
From `properties`: `gridId` (e.g. "VEF"), `gridX`, `gridY`, `timeZone`,
`observationStations` (URL). Cache gridId/gridX/gridY in NVS beside the zip.
Response ~3KB — fits existing 32KB buffer.

### 2. One-time per zip: nearest station
GET the `observationStations` URL → `features[0].properties.stationIdentifier`
(e.g. "KVGT"). Cache in NVS. Response can be ~20KB (list of stations) — only
need the first feature; truncate-and-repair trick (below) or just find
`"stationIdentifier":"..."` with strstr — first occurrence is the nearest.

### 3. Current conditions — every 15 min
`https://api.weather.gov/stations/KVGT/observations/latest`
From `properties`:
- `temperature.value` — **°C, may be null** (station gap). Convert F = C*9/5+32.
- `relativeHumidity.value` — %, may be null
- `windSpeed.value` — **km/h**, may be null → mph = v / 1.609
- `windDirection.value` — degrees
- `heatIndex.value` / `windChill.value` — °C or null → feels-like; fall back to temp
- `textDescription` — "Sunny", "Partly Cloudy" → display status text directly
- `icon` — URL like `https://api.weather.gov/icons/land/day/skc?...`
  - `/day/` vs `/night/` → is_day
  - last path token (e.g. `skc`) → icon keyword (mapping table below)
Response ~5KB. NULL HANDLING IS MANDATORY — every .value can be null (cJSON_IsNumber
check). On null temp: keep previous value, or fall back to hourly period[0].

### 4. Hourly forecast — every hour
`https://api.weather.gov/gridpoints/VEF/{x},{y}/forecast/hourly?units=us`
`properties.periods[]` (156 entries, response **60–70KB** — see Memory):
- `startTime` — ISO8601 WITH OFFSET e.g. "2026-06-11T06:00:00-07:00"
  → parse the `-07:00` suffix for utc_offset_seconds (replaces Open-Meteo's
  utc_offset_seconds; feeds the home-screen clock!)
- `temperature` — integer, already °F with units=us
- `probabilityOfPrecipitation.value` — % (period[0] = "Chance Rain" display)
- `icon`, `isDaytime`, `shortForecast`
Display uses periods[1..4] for the 4 hourly slots (period[0] = current hour).

### 5. Daily forecast — every hour (same fetch cycle)
`https://api.weather.gov/gridpoints/VEF/{x},{y}/forecast?units=us`
`properties.periods[]` — ~14 entries, DAY/NIGHT PAIRS (~10–15KB, fits buffer):
- Day period: `name`="Thursday", `temperature`=HIGH, `isDaytime`=true
- Night period: `name`="Thursday Night", `temperature`=LOW
- First period may be "Today"/"This Afternoon"/"Tonight" — handle specially.
Pairing logic: walk periods; for each isDaytime=true period take temp as high,
next period temp as low, day name = first 3 chars of `name`. Skip today —
display shows the NEXT 3 days (per June 10 fix/user requirement).
Edge: if first period is "Tonight" (evening fetch), today has no day-period;
pairing still works — just skip until tomorrow's day period.

## Icon keyword → existing icon set
NWS icon keywords (last URL path segment, strip query/size). Map to the WMO
codes already used by wmo_icon_map / weather_icons so NO image changes needed:
| NWS keyword | WMO code to feed existing pipeline |
|---|---|
| skc | 0 (clear) |
| few | 1 |
| sct | 2 |
| bkn | 3 |
| ovc | 3 |
| wind_skc/few/sct/bkn/ovc | same as base |
| fog | 45 |
| haze, smoke, dust | 45 |
| rain_showers, rain_showers_hi | 80 |
| rain | 61 |
| tsra, tsra_sct, tsra_hi | 95 |
| snow, rain_snow, snow_sleet | 71 |
| sleet, fzra, rain_fzra | 66 |
| hot, cold | 0 |
| blizzard | 73 |
| tornado, hurricane, tropical_storm | 95 |
Unknown keyword → 0 + log warning. Keep `textDescription`/`shortForecast` as the
status string instead of wmo_get_description (NWS text is better).

## Memory strategy (the one real engineering problem)
The hourly forecast response is 60–70KB; current buffer is 32KB internal RAM.
1. Allocate the HTTP response buffer from **PSRAM**:
   `heap_caps_malloc(128*1024, MALLOC_CAP_SPIRAM)` — board has 32MB, video
   already uses 15MB. Free after parse.
2. DO NOT cJSON-parse 70KB (node overhead would strain internal heap).
   **Truncate-and-repair:** find `"periods":[`, then scan forward counting
   matching braces to the end of the Nth period object (N=6). Cut there,
   append `]}}`, cJSON_Parse the resulting ~4KB. Structure is
   `{"properties":{"periods":[{...},{...}]}}` so the repair suffix is fixed.
   (Simple brace-depth counter; NWS strings don't contain braces, but guard
   for `"` toggling in-string state anyway.)
3. The daily /forecast (~12KB) and observation (~5KB) responses fit the
   existing 32KB path unchanged, but may as well share the PSRAM buffer.

## Fetch cadence / task changes (weather_task.c)
- Observations: every 15 min (real measured temp 4×/hour — REPLACES the
  minutely_15 step-through machinery entirely; delete the overlay code).
- Hourly + daily forecast + precip prob: every 60 min.
- Implementation: keep one task; tick every 15 min; do forecast fetches when
  60-min counter expires. last_update bumps on every obs change → existing
  main.c change-detect drives UI refresh untouched.
- Keep NVS caching of last-good data for offline boot (existing pattern).
- /points + station lookup: only on zip change (or NVS miss). Cache:
  nws_grid_id (str), nws_grid_x/y (i32), nws_station (str).

## What gets deleted
- All Open-Meteo URL building + parse (parse_weather_response minutely/hourly/
  daily sections), minutely_step_t machinery, the weather_get_data overlay,
  WMO-code-from-Open-Meteo path (codes now come from icon keyword mapping).
- Keep: zippopotam geocode, http_get helper (add PSRAM buffer variant),
  weather_data_t shape (display contract), NVS weather cache, task skeleton.

## File plan
- NEW `main/weather/nws_api.c/h` — points/station/obs/hourly/daily fetchers + parsers
- `weather_api.c` — shrinks to zippopotam geocode + shared http_get
- `weather_task.c` — cadence change, calls nws_* functions
- `weather_data.h` — add char status_text[32]; keep struct otherwise (display contract)
- `wmo_icon_map` — unchanged (fed by the keyword table)

## Testing checklist (Claude Code: build/flash/monitor loop)
1. Browser-verify all 5 endpoints for 36.2589,-115.1718 first; paste real JSON
   into the session so the parser matches reality, not guesses.
2. Boot → log grid VEF/x,y + station KVGT cached
3. Obs temp within ~2°F of AccuWeather/thermometer ← THE acceptance test
4. Null-temp observation handled (simulate: parse a doctored payload)
5. Hourly slots show next 4 hours, daily rows show Thu/Fri/Sat (not today)
6. Clock correct (offset parsed from startTime)
7. Evening fetch: first daily period = "Tonight" pairing edge case
8. WiFi drop → NVS cache fallback still works
9. Heap check: INT free stable across 10 fetch cycles (mem_label timer exists)

## Proxy note (later, when device #2 exists)
All of this moves server-side per WEATHER_PROXY_CACHE_PLAN.md — proxy calls NWS,
caches per station/grid, devices get one merged JSON. Firmware then shrinks again.
