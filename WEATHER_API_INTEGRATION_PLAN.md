# Weather API Integration Plan
## Open-Meteo → SquareLine GUI Data Flow

---

## 1. Open-Meteo API Response Structure

### Current Weather Endpoint
```
GET https://api.open-meteo.com/v1/forecast
?latitude=39.7392
&longitude=-104.9903
&current=temperature_2m,relative_humidity_2m,weather_code,precipitation_probability
&hourly=temperature_2m,weather_code,precipitation_probability
&daily=temperature_2m_max,temperature_2m_min,weather_code,precipitation_probability_max
&timezone=auto
```

### Response JSON
```json
{
  "current": {
    "temperature_2m": 72.5,
    "relative_humidity_2m": 63,
    "weather_code": 2,
    "precipitation_probability": 12,
    "time": "2026-06-02T14:34:00"
  },
  "hourly": {
    "time": ["2026-06-02T14:00", "2026-06-02T15:00", ...],
    "temperature_2m": [72.5, 71.2, 70.1, ...],
    "weather_code": [2, 2, 3, 1, 0, ...],
    "precipitation_probability": [12, 15, 10, 5, 0, ...]
  },
  "daily": {
    "time": ["2026-06-02", "2026-06-03", "2026-06-04"],
    "temperature_2m_max": [78, 76, 75],
    "temperature_2m_min": [65, 63, 62],
    "weather_code": [2, 3, 1],
    "precipitation_probability_max": [15, 20, 10]
  },
  "timezone": "America/Denver"
}
```

---

## 2. WMO Code → AccuWeather Icon Mapping

### Quick Reference Table

| WMO Code | Description | AccuWeather Icon | Icon File (60x60) | Icon File (22x22) |
|----------|-------------|------------------|-------------------|-------------------|
| 0 | Clear sky | 1 (day) / 33 (night) | UI/60x60/1.png | UI/22x22/1s.png |
| 1 | Mainly sunny/clear | 2 (day) / 34 (night) | UI/60x60/2.png | UI/22x22/2s.png |
| 2 | Partly cloudy | 3 (day) / 35 (night) | UI/60x60/3.png | UI/22x22/3s.png |
| 3 | Cloudy | 6-8 (day) / 36-38 (night) | UI/60x60/6.png | UI/22x22/6s.png |
| 45, 48 | Foggy | 11 (both) | UI/60x60/11.png | UI/22x22/11s.png |
| 51-57 | Drizzle | 12-14 (day) / 39-40 (night) | UI/60x60/12.png | UI/22x22/12s.png |
| 61-67 | Rain | 18 (both) | UI/60x60/18.png | UI/22x22/18s.png |
| 66-67 | Freezing rain | 26 (both) | UI/60x60/26.png | UI/22x22/26s.png |
| 71-77 | Snow | 22 (day) / 44 (night) | UI/60x60/22.png | UI/22x22/44s.png |
| 80-82 | Showers | 12-14 (day) / 39-40 (night) | UI/60x60/12.png | UI/22x22/12s.png |
| 85-86 | Snow showers | 19 (day) / 43 (night) | UI/60x60/19.png | UI/22x22/43s.png |
| 95-99 | Thunderstorm | 15 (day) / 41-42 (night) | UI/60x60/15.png | UI/22x22/15s.png |
| 611-613 | Sleet | 25 (both) | UI/60x60/25.png | UI/22x22/25s.png |
| 615-616 | Rain & snow | 29 (both) | UI/60x60/29.png | UI/22x22/29s.png |

---

## 3. GUI Data Binding Points

### Home Screen (Weather Display)

```c
// Current Weather Section
GUI_Label__home__CURRENTTIMEQ         // "2:34 PM" or "14:34"
GUI_Label__home__LOCATIONQ            // "Denver, CO"
GUI_Label__home__CURRENTTEMPQ         // "72°F" or "22°C"
GUI_Image__home__60X60ICONQ           // Weather icon (60x60px)
GUI_Label__home__CURRENTSTATUSQ       // "Partly Cloudy"

// Current Conditions Row
GUI_Label__home__HUMIDITYQ            // "63%" (humidity)
GUI_Label__home__RAINPERCENT_Q        // "12%" (rain probability)
GUI_Label__home__WINDQ                // "12 mph" (wind speed - from separate API)

// High/Low for Today
GUI_Label__home__TODAYHIGHQ           // "78°F"
GUI_Label__home__TODAYLOWQ            // "65°F"

// 5-Hour Hourly Forecast (repeated 5 times)
GUI_Label__home__HOUR1TIMEQ           // "3 PM"
GUI_Image__home__HOUR1ICONQ           // Weather icon (22x22px)
GUI_Label__home__HOUR1TEMPQ           // "71°F"
GUI_Label__home__HOUR1RAINQ           // "15%"
// ... repeat for hours 2-5
```

### Settings Screen
```c
GUI_Label__settings__ZIPCODE_Q        // Current ZIP for reference
GUI_Label__settings__CITY_Q           // Current city name
GUI_Label__settings__UNIT_Q           // °F or °C toggle
```

---

## 4. Data Flow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                     Weather Update Cycle                         │
└─────────────────────────────────────────────────────────────────┘

1. User enters ZIP code in keyboardzipcode screen
   ↓
2. On ZIP submit:
   - Call geocoding API (OpenCage/Google)
   - Get: latitude, longitude, city_name, state_name
   - Store in NVS: "lat", "lon", "city", "state"
   ↓
3. Switch to home screen
   ↓
4. Every 15 minutes (background task):
   a. Read NVS: latitude, longitude
   b. Call Open-Meteo API
   c. Parse JSON response:
      - current: temp, humidity, wmo_code, precip_prob
      - hourly: next 5 hours of temp, wmo_code, precip_prob
      - daily: next 3 days of high, low, wmo_code
   d. Call update_weather_display() with parsed data
   e. Update lv_label and lv_image widgets
   ↓
5. Display updates on screen (non-blocking)
```

---

## 5. C Functions Needed

### Weather Data Structure
```c
typedef struct {
    float current_temp;
    float current_humidity;
    int current_weather_code;
    int current_precip_prob;
    float current_wind_speed;
    
    float daily_high[3];
    float daily_low[3];
    int daily_weather_code[3];
    int daily_precip_prob[3];
    
    float hourly_temp[5];
    int hourly_weather_code[5];
    int hourly_precip_prob[5];
    char hourly_time[5][10];  // "3 PM", "4 PM", etc.
    
    char city[50];
    char state[50];
    char timezone[50];
    time_t last_update;
} weather_data_t;
```

### Icon Selection Function
```c
/**
 * Map WMO code to AccuWeather icon number
 * @param wmo_code: WMO weather code (0-99)
 * @param is_night: true if nighttime, false if day
 * @return: AccuWeather icon number (1-44)
 */
int wmo_to_accuweather_icon(int wmo_code, bool is_night);

/**
 * Get icon file path from AccuWeather icon number
 * @param icon_num: AccuWeather icon number (1-44)
 * @param size: "60x60" or "22x22"
 * @return: pointer to icon filename (e.g., "UI/60x60/2.png")
 */
const char* get_icon_path(int icon_num, const char* size);
```

### GUI Update Function
```c
/**
 * Update home screen with weather data
 * @param weather: pointer to weather_data_t struct
 * @param is_night: true if current time is after sunset
 */
void update_weather_display(weather_data_t* weather, bool is_night);
```

### Implementation Outline
```c
int wmo_to_accuweather_icon(int wmo_code, bool is_night) {
    switch(wmo_code) {
        case 0: return is_night ? 33 : 1;    // Clear
        case 1: return is_night ? 34 : 2;    // Mainly sunny
        case 2: return is_night ? 35 : 3;    // Partly cloudy
        case 3: return is_night ? 36 : 6;    // Cloudy
        case 45: case 48: return 11;         // Fog
        case 51: case 53: case 55: return is_night ? 39 : 12;  // Drizzle
        case 61: case 63: case 65: return 18; // Rain
        case 66: case 67: return 26;         // Freezing rain
        case 71: case 73: case 75: case 77: return is_night ? 44 : 22;  // Snow
        case 80: case 81: case 82: return is_night ? 40 : 14;  // Showers
        case 85: case 86: return is_night ? 43 : 19;  // Snow showers
        case 95: case 96: case 99: return is_night ? 41 : 15;  // Thunderstorm
        case 611: case 612: case 613: return 25;  // Sleet
        case 615: case 616: return 29;           // Rain & snow
        default: return 1;  // Default to sunny
    }
}

const char* get_icon_path(int icon_num, const char* size) {
    static char path[64];
    snprintf(path, sizeof(path), "UI/%s/%d%s.png", 
             size, icon_num, 
             strcmp(size, "22x22") == 0 ? "s" : "");
    return path;
}

void update_weather_display(weather_data_t* weather, bool is_night) {
    char buffer[50];
    
    // Update current conditions
    snprintf(buffer, sizeof(buffer), "%.0f°F", weather->current_temp);
    lv_label_set_text(GUI_Label__home__CURRENTTEMPQ, buffer);
    
    lv_label_set_text(GUI_Label__home__LOCATIONQ, weather->city);
    
    // Update icon
    int icon_num = wmo_to_accuweather_icon(weather->current_weather_code, is_night);
    lv_image_set_src(GUI_Image__home__60X60ICONQ, 
                     get_icon_path(icon_num, "60x60"));
    
    // Update status text
    lv_label_set_text(GUI_Label__home__CURRENTSTATUSQ, 
                      get_weather_description(weather->current_weather_code));
    
    // Update humidity and rain
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_humidity);
    lv_label_set_text(GUI_Label__home__HUMIDITYQ, buffer);
    
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_precip_prob);
    lv_label_set_text(GUI_Label__home__RAINPERCENT_Q, buffer);
    
    // Update 5-hour forecast
    for(int i = 0; i < 5; i++) {
        snprintf(buffer, sizeof(buffer), "%.0f°F", weather->hourly_temp[i]);
        // Update hour i temperature
        
        int hour_icon = wmo_to_accuweather_icon(
            weather->hourly_weather_code[i], is_night);
        // Update hour i icon
        
        snprintf(buffer, sizeof(buffer), "%d%%", weather->hourly_precip_prob[i]);
        // Update hour i rain %
    }
}
```

---

## 6. Data Sync Checklist

- [ ] **Icon Numbering**: AccuWeather icons 1-44 match PNG filenames in UI/60x60/ and UI/22x22/s
- [ ] **WMO Mapping**: All 28 WMO codes mapped to AccuWeather equivalents
- [ ] **Day/Night Logic**: Sunrise/sunset times used to select day vs. night icons
- [ ] **GUI Labels**: All "Q" marked labels identified and mapped to data fields
- [ ] **Update Frequency**: 15-minute interval doesn't block audio or display
- [ ] **Timezone Handling**: Local time conversion for hourly forecast ("3 PM" not "15:00")
- [ ] **Error Handling**: API failures don't crash app, show cached data or "unavailable"
- [ ] **NVS Storage**: ZIP code, city, state persisted across power cycles

---

## 7. Integration Sequence

1. **Geocoding Setup** (Phase 1)
   - Add OpenCage API key
   - Create zip_to_coords() function
   - Test with ZIP code input

2. **Weather API Task** (Phase 2)
   - Create weather_update_task() FreeRTOS task
   - Fetch Open-Meteo data every 15 minutes
   - Parse JSON response
   - Store in weather_data_t struct

3. **GUI Updates** (Phase 3)
   - Implement wmo_to_accuweather_icon()
   - Implement update_weather_display()
   - Hook weather_data_t to GUI widgets

4. **Testing** (Phase 4)
   - Mock weather data for different conditions
   - Test all 44 icon displays
   - Verify day/night switching
   - Test offline fallback

---

## Example Test Cases

```c
// Test case 1: Clear sunny day
weather_data_t test1 = {
    .current_temp = 75,
    .current_weather_code = 0,  // Clear sky
    .city = "Denver, CO",
    // ...
};
// Expected icon: UI/60x60/1.png (sunny day icon)

// Test case 2: Rainy night
weather_data_t test2 = {
    .current_temp = 65,
    .current_weather_code = 63,  // Rain
    .city = "Seattle, WA",
    // ...
};
// Expected icon: UI/60x60/18.png (rain icon)

// Test case 3: Snowy night
weather_data_t test3 = {
    .current_temp = 28,
    .current_weather_code = 73,  // Snow
    .city = "Denver, CO",
    // ...
};
// Expected icon: UI/60x60/44.png (snow night icon) or UI/60x60/22.png (day)
```

---

## Summary

✅ **All pieces align:**
- WMO codes → AccuWeather icon numbers
- Icon numbers → PNG files (60x60 and 22x22)
- Open-Meteo API response → weather_data_t struct
- weather_data_t → GUI widgets (labels + images)
- 15-minute update cycle → non-blocking FreeRTOS task

**Ready to start integration!**
