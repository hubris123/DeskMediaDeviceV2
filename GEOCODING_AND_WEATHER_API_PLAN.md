# Geocoding + Weather API Integration Plan
## Zippopotam.us + Open-Meteo Complete Workflow

---

## 1. Zippopotam.us Geocoding API

### Endpoint
```
https://api.zippopotam.us/us/{zipcode}
```

### Request Example
```
GET https://api.zippopotam.us/us/80202
```

### Response Example
```json
{
  "post code": "80202",
  "country": "United States",
  "country abbreviation": "US",
  "places": [
    {
      "place name": "Denver",
      "longitude": "-104.9903",
      "state": "CO",
      "state abbreviation": "CO",
      "latitude": "39.7392"
    }
  ]
}
```

### Key Fields We Extract
```c
char city[50];           // "Denver"
char state[10];          // "CO"
float latitude;          // 39.7392
float longitude;         // -104.9903
```

---

## 2. Open-Meteo Weather API

### Endpoint (after getting coordinates from Zippopotam)
```
https://api.open-meteo.com/v1/forecast
?latitude={latitude}
&longitude={longitude}
&current=temperature_2m,relative_humidity_2m,weather_code,precipitation_probability
&hourly=temperature_2m,weather_code,precipitation_probability,time
&daily=temperature_2m_max,temperature_2m_min,weather_code,precipitation_probability_max,time
&timezone=auto
&timeformat=unixtime
```

### Example (for Denver)
```
GET https://api.open-meteo.com/v1/forecast?latitude=39.7392&longitude=-104.9903&current=...
```

---

## 3. Complete Data Flow

```
┌─────────────────────────────────────────────────────────────────────┐
│                     Complete Weather Update Workflow                 │
└─────────────────────────────────────────────────────────────────────┘

1. USER INPUT PHASE
   ├─ User navigates to Settings → Zip Code Keyboard
   ├─ Enters ZIP code: "80202"
   └─ Presses Submit

2. GEOCODING PHASE
   ├─ Call Zippopotam.us API: /us/80202
   ├─ Parse response:
   │  ├─ city = "Denver"
   │  ├─ state = "CO"
   │  ├─ latitude = 39.7392
   │  └─ longitude = -104.9903
   ├─ Store in NVS:
   │  ├─ "city" → "Denver"
   │  ├─ "state" → "CO"
   │  ├─ "lat" → 39.7392
   │  └─ "lon" → -104.9903
   └─ Switch to Home screen

3. WEATHER FETCHING PHASE (First request + 15-min loop)
   ├─ Read NVS coordinates (lat, lon)
   ├─ Build Open-Meteo URL with coordinates
   ├─ Call Open-Meteo API
   ├─ Parse JSON response:
   │  ├─ current: temp, humidity, wmo_code, precip_prob
   │  ├─ hourly: next 5 hours of temps, codes, precip
   │  └─ daily: next 3 days of highs, lows, codes, precip
   └─ Store in weather_data_t struct

4. GUI UPDATE PHASE
   ├─ Call update_weather_display(weather_data)
   ├─ Extract WMO code → AccuWeather icon number
   ├─ Load icon PNG from UI/{size}/{icon_num}s.png
   └─ Update all "Q" marked labels and images on home screen

5. REPEAT EVERY 15 MINUTES
   └─ Go to step 3 (weather fetching only, not geocoding)
```

---

## 4. C Data Structures

### Geocoding Response
```c
typedef struct {
    char city[50];
    char state[10];
    float latitude;
    float longitude;
} location_t;
```

### Weather Data
```c
typedef struct {
    // Current conditions
    float current_temp;
    float current_humidity;
    int current_weather_code;
    int current_precip_prob;
    
    // Hourly (next 5 hours)
    float hourly_temp[5];
    int hourly_weather_code[5];
    int hourly_precip_prob[5];
    time_t hourly_time[5];
    
    // Daily (next 3 days)
    float daily_high[3];
    float daily_low[3];
    int daily_weather_code[3];
    int daily_precip_prob[3];
    time_t daily_time[3];
    
    // Location info
    char city[50];
    char state[10];
    float latitude;
    float longitude;
    char timezone[50];
    
    time_t last_update;
} weather_data_t;
```

---

## 5. C Functions Needed

### Step 1: Fetch Coordinates from Zippopotam
```c
/**
 * Convert ZIP code to coordinates
 * @param zipcode: "80202"
 * @param location: pointer to location_t struct to fill
 * @return: true if success, false if API error
 */
bool fetch_coordinates(const char* zipcode, location_t* location) {
    // 1. Build URL: "https://api.zippopotam.us/us/{zipcode}"
    // 2. HTTP GET request via WiFi
    // 3. Parse JSON response
    // 4. Extract: city, state, latitude, longitude
    // 5. Store in location_t
    // 6. Save to NVS for persistence
    return true;
}

// Example usage:
location_t loc;
if (fetch_coordinates("80202", &loc)) {
    printf("City: %s, State: %s\n", loc.city, loc.state);
    printf("Lat: %.4f, Lon: %.4f\n", loc.latitude, loc.longitude);
    // Now use loc.latitude and loc.longitude for weather API
}
```

### Step 2: Fetch Weather from Open-Meteo
```c
/**
 * Fetch weather data for coordinates
 * @param location: pointer to location_t with lat/lon
 * @param weather: pointer to weather_data_t struct to fill
 * @return: true if success, false if API error
 */
bool fetch_weather(const location_t* location, weather_data_t* weather) {
    // 1. Build URL with location->latitude, location->longitude
    // 2. Add query params: current, hourly, daily, timezone=auto
    // 3. HTTP GET request via WiFi
    // 4. Parse JSON response
    // 5. Extract all fields into weather_data_t
    // 6. Copy city/state from location
    return true;
}

// Example usage:
weather_data_t weather;
if (fetch_weather(&loc, &weather)) {
    printf("Temp: %.1f°F\n", weather.current_temp);
    printf("Humidity: %d%%\n", weather.current_humidity);
    printf("WMO Code: %d\n", weather.current_weather_code);
}
```

### Step 3: Update GUI with Weather Data
```c
/**
 * Update home screen with weather data
 * @param weather: pointer to weather_data_t
 */
void update_weather_display(const weather_data_t* weather) {
    char buffer[50];
    
    // Current location
    snprintf(buffer, sizeof(buffer), "%s, %s", weather->city, weather->state);
    lv_label_set_text(GUI_Label__home__LOCATIONQ, buffer);
    
    // Current temperature
    snprintf(buffer, sizeof(buffer), "%.0f°F", weather->current_temp);
    lv_label_set_text(GUI_Label__home__CURRENTTEMPQ, buffer);
    
    // Current humidity
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_humidity);
    lv_label_set_text(GUI_Label__home__HUMIDITYPERCENTQ, buffer);
    
    // Chance of rain
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_precip_prob);
    lv_label_set_text(GUI_Label__home__CHANCEOFRAINQ, buffer);
    
    // Weather icon (convert WMO to AccuWeather icon)
    int icon_num = wmo_to_accuweather_icon(weather->current_weather_code, 
                                           is_nighttime(weather));
    lv_image_set_src(GUI_Image__home__60X60ICONQ, 
                     get_icon_path(icon_num, "60x60"));
    
    // 5-hour forecast
    for (int i = 0; i < 5; i++) {
        // Time
        struct tm* tm_info = gmtime(&weather->hourly_time[i]);
        strftime(buffer, sizeof(buffer), "%I %p", tm_info);
        // Update HR{i+1}FORCASTTIMEQ
        
        // Temp
        snprintf(buffer, sizeof(buffer), "%.0f°F", weather->hourly_temp[i]);
        // Update HR{i+1}FORCASTTEMPQ
        
        // Icon
        int hr_icon = wmo_to_accuweather_icon(weather->hourly_weather_code[i], 
                                              is_nighttime(weather));
        // Update HR{i+1}FORCASICON22X22Q
    }
    
    // 3-day forecast (similar pattern)
    for (int i = 0; i < 3; i++) {
        // Day, high, low, condition, icon...
    }
}
```

### Step 4: Background Task (Every 15 minutes)
```c
void weather_update_task(void *pvParameters) {
    location_t location;
    weather_data_t weather;
    
    // Get stored coordinates from NVS
    nvs_get_f32(nvs_handle, "lat", &location.latitude);
    nvs_get_f32(nvs_handle, "lon", &location.longitude);
    nvs_get_str(nvs_handle, "city", location.city, sizeof(location.city));
    nvs_get_str(nvs_handle, "state", location.state, sizeof(location.state));
    
    while (1) {
        // Fetch fresh weather data
        if (fetch_weather(&location, &weather)) {
            // Update GUI
            update_weather_display(&weather);
            
            // Store for offline fallback
            save_weather_to_nvs(&weather);
        } else {
            // Load cached weather if API fails
            load_weather_from_nvs(&weather);
            update_weather_display(&weather);
        }
        
        // Wait 15 minutes (900 seconds)
        vTaskDelay(900000 / portTICK_PERIOD_MS);
    }
}
```

---

## 6. Integration Sequence

### Phase 1: Settings Screen → Zip Entry
- User navigates to keyboardzipcode screen
- Enters ZIP code
- Presses Submit
- Call `fetch_coordinates(zipcode, &location)`
- Save location to NVS
- Display confirmation
- Switch to home screen

### Phase 2: Home Screen → First Weather Update
- Create background task: `xTaskCreate(weather_update_task, ...)`
- Task reads coordinates from NVS
- Calls `fetch_weather(&location, &weather)`
- Calls `update_weather_display(&weather)`
- Display shows current conditions + forecast

### Phase 3: Continuous Updates
- Every 15 minutes, task repeats steps from Phase 2
- User can manually refresh by pressing button (if implemented)
- Weather data persists across power cycles via NVS

---

## 7. HTTP Implementation Notes

### ESP32 HTTP Libraries
```c
#include "esp_http_client.h"  // Built-in ESP-IDF library

// Simple GET request pattern:
esp_http_client_config_t config = {
    .url = "https://api.zippopotam.us/us/80202",
};

esp_http_client_handle_t client = esp_http_client_init(&config);
esp_http_client_perform(client);
int status_code = esp_http_client_get_status_code(client);
// Read response body
esp_http_client_cleanup(client);
```

### JSON Parsing
```c
#include "cJSON.h"  // Or esp_http_server built-in parser

// Parse Zippopotam response:
cJSON *root = cJSON_Parse(response_buffer);
cJSON *places = cJSON_GetObjectItem(root, "places");
cJSON *place = cJSON_GetArrayItem(places, 0);

const char *city = cJSON_GetObjectItem(place, "place name")->valuestring;
const char *state = cJSON_GetObjectItem(place, "state")->valuestring;
float lat = cJSON_GetObjectItem(place, "latitude")->valuedouble;
float lon = cJSON_GetObjectItem(place, "longitude")->valuedouble;

cJSON_Delete(root);
```

---

## 8. Error Handling

```c
// WiFi availability check
if (!is_wifi_connected()) {
    // Use cached weather from NVS
    load_weather_from_nvs(&weather);
    update_weather_display(&weather);
    return;
}

// API timeout
if (http_get_timeout_exceeded()) {
    // Use cached weather
    load_weather_from_nvs(&weather);
    return;
}

// Invalid ZIP code
if (fetch_coordinates(zipcode, &location) == false) {
    // Show error on keyboardzipcode screen
    lv_label_set_text(error_label, "Invalid ZIP Code");
    return;
}

// Stale data fallback (if API down for hours)
time_t now = time(NULL);
if (now - weather.last_update > 3600) {  // > 1 hour old
    // Show "Last updated: X hours ago" indicator
}
```

---

## 9. Data Sync Checklist

- [ ] **Zippopotam API**: ZIP → city, state, lat, lon ✅
- [ ] **Open-Meteo API**: lat, lon → weather (current, hourly, daily)
- [ ] **WMO mapping**: All 28 codes → AccuWeather icons
- [ ] **Icon loading**: 60x60 main + 22x22 hourly from PNG files
- [ ] **GUI updates**: All Q-marked labels populated
- [ ] **NVS storage**: Location persisted across power cycles
- [ ] **Weather caching**: Fallback if WiFi/API fails
- [ ] **Timezone handling**: Local time for hourly/daily display
- [ ] **15-minute loop**: Non-blocking background task
- [ ] **Error messages**: User feedback for invalid ZIP or API errors

---

## 10. Testing Plan

### Test Case 1: ZIP Code Entry
```
Input: "80202"
Expected: "Denver, CO" stored in NVS
Verify: Settings shows correct location
```

### Test Case 2: Weather Fetch
```
Input: Valid coordinates (Denver)
Expected: Current temp, humidity, rain %, weather icon displayed
Verify: All Q-marked labels populated correctly
```

### Test Case 3: Hourly Forecast
```
Input: 5-hour forecast data from API
Expected: Time, temp, icon for each hour
Verify: 22x22 icons load correctly (UI/22x22/*s.png)
```

### Test Case 4: Daily Forecast
```
Input: 3-day forecast data
Expected: High/low temps, weather icon, day name
Verify: Icons and temps match data
```

### Test Case 5: Offline Fallback
```
Disconnect WiFi
Expected: Show cached weather from last update
Verify: Data doesn't disappear, user sees stale timestamp
```

---

## Summary

✅ **Complete workflow from ZIP code → weather display:**
1. User enters ZIP in settings
2. Zippopotam API → coordinates + city/state
3. Open-Meteo API → current + hourly + daily weather
4. WMO code → AccuWeather icon mapping
5. GUI update with all data
6. Every 15 minutes: repeat steps 3-5
7. Offline fallback: use cached data

**All pieces ready for main.c integration!**
