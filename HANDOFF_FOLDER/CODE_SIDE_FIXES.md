# Code-Side Fixes Applied After SquareLine Export
**File: main/main.c — inside create_ui() after GUI_init()**

These fixes must be re-applied in code every session because SquareLine resets them on export.

---

## 1. Background Image
```c
lv_obj_set_style_bg_image_src(GUI_Screen__home,
    &upload_hclbg1_52bba57ce173452fadd7595a14167a99_png, 0);
```

## 2. Panel 3 Height Fix
SquareLine resets this to 210 on every export — must force to 220:
```c
lv_obj_set_size(GUI_Panel__home__panel_3, 60, 220);
```

## 3. Music Player — Parked Off Screen on Boot
```c
lv_obj_set_y(GUI_Container__home__music_player, MUSIC_PLAYER_Y_HIDDEN);  // 400
lv_obj_set_x(GUI_Container__home__musicbuttoncont, MUSICBTN_X_REST);      // -132
```

## 4. Scroll Panel — Parked Off Screen on Boot
```c
lv_obj_set_y(GUI_Panel__home__musicscrollbg, 255); // just below bottom of 480h canvas
```

## 5. Scroll Panel Styling (overrides SquareLine white default)
```c
lv_obj_set_style_bg_color(GUI_Panel__home__musicscrollbg,
    lv_color_hex(0x222222), LV_PART_MAIN);
lv_obj_set_style_bg_opa(GUI_Panel__home__musicscrollbg,
    LV_OPA_COVER, LV_PART_MAIN);
lv_obj_set_style_text_color(GUI_Label__home__musicscrolllabel,
    lv_color_hex(0xFFFFFF), LV_PART_MAIN);
```

## 6. Scroll Label Size and Position
```c
lv_label_set_long_mode(GUI_Label__home__musicscrolllabel, LV_LABEL_LONG_CLIP);
lv_obj_set_size(GUI_Panel__home__musicscrollbg, 424, 26);  // 2px taller than SL default
lv_obj_set_pos(GUI_Label__home__musicscrolllabel, 4, 1);   // inset 4px left, 1px down
lv_obj_set_width(GUI_Label__home__musicscrolllabel, 416);
```

## 7. Precipitation Label Renamed
```c
lv_label_set_text(GUI_Label__home__Precipitation, "Chance Rain");
```

## 8. Wind Default Fix (SL hardcodes "12")
In `ui_set_default_weather()` in `main/ui/data_binding.c`:
```c
lv_label_set_text(GUI_Label__home__details_wind_speed_lbl, "--");
```

## 9. Button Event Callbacks
```c
lv_obj_add_event_cb(GUI_Button__home__buttonplay,
    music_buttonplay_cb, LV_EVENT_CLICKED, NULL);
lv_obj_add_event_cb(GUI_Button__home__buttonup,
    music_buttonup_cb, LV_EVENT_CLICKED, NULL);
lv_obj_add_event_cb(GUI_Button__home__buttondown,
    music_buttondown_cb, LV_EVENT_CLICKED, NULL);
lv_obj_add_event_cb(GUI_Button__home__musicbuttongrey,
    music_player_btn_cb, LV_EVENT_CLICKED, NULL);
```

## 10. LVGL Timers Created in create_ui()
```c
lv_timer_create(music_watchdog_cb, 500, NULL);   // detects track end
lv_timer_create(clock_timer_cb, 1000, NULL);     // 1-second clock update
lv_timer_create(mem_label_cb, 1000, NULL);       // memory display (hidden)
// scroll_timer created dynamically in music_scroll_start()
```

## 11. Memory Label (hidden — remove flag to show)
```c
mem_label = lv_label_create(GUI_Screen__home);
lv_obj_set_style_text_font(mem_label, &lv_font_montserrat_12, LV_PART_MAIN);
lv_obj_set_style_text_color(mem_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
lv_obj_set_style_bg_opa(mem_label, LV_OPA_TRANSP, LV_PART_MAIN);
lv_obj_set_align(mem_label, LV_ALIGN_BOTTOM_LEFT);
lv_obj_set_pos(mem_label, 2, -18);
lv_label_set_text(mem_label, "INT:--- PSRAM:---");
lv_obj_add_flag(mem_label, LV_OBJ_FLAG_HIDDEN); // remove this line to show
```

---

## Key Constants (top of main.c)
```c
#define AUDIO_SAMPLE_RATE       44100  // default; mp3_task reconfigures per-file
#define MUSIC_PLAYER_Y_REST     115    // center-relative y when player is open
#define MUSIC_PLAYER_Y_HIDDEN   400    // off bottom of 480h canvas
#define MUSICBTN_X_REST        -132    // same x as music_player (next to weather widget)
#define MUSICBTN_X_OPEN         -68    // shifted right when player is open
#define ML_W                    424    // scroll panel width for animation
```

---

## After Every SquareLine Export — Checklist
1. Run `fix_ui_export.py`
2. Verify `global_styles.c` ends with `}`
3. Re-copy `upload_hclbg1_...png.c` background image asset if UI was swapped
4. Check `panel_3` height is 220 (SL resets to 210)
5. Merge new assets into `CMakeLists.txt` if new images were added
6. Check `variables.h` and `variables.c` for new widget declarations
