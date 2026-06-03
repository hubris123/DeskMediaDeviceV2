# SquareLine UI Integration Review

**Status:** ✅ Ready for integration with minor adjustments needed

---

## Project Structure Review

### ✅ Strengths
- **Well-organized**: Proper separation of concerns (screens, components, assets, core)
- **5 Screens implemented**: home, keyboardpassword, keyboardzipcode, settingswindow, networkselector
- **Custom assets**: 7 fonts + 5 images embedded
- **Modular design**: Components and hooks for reusability
- **LVGL 9.x compatible**: Uses modern LVGL API (lv_display_t, lv_image_create, etc.)
- **CMakeLists.txt ready**: Configured as a library - easy to integrate

### Screens Found
1. **home** - Main weather display (weather widget)
2. **settingswindow** - Settings menu
3. **keyboardpassword** - Password input keyboard
4. **keyboardzipcode** - ZIP code input keyboard
5. **networkselector** - Network/WiFi selection

---

## Integration Checklist

### 🔴 CRITICAL - Must Fix Before Build

#### 1. **Verify LVGL Version**
- ✅ Code uses LVGL 9.x API (lv_display_get_default, lv_image_create)
- Need to verify: Is sdkconfig set to LVGL 9.4?
- **Action:** Check `sdkconfig` for `CONFIG_LV_VERSION_*`

#### 2. **HAL Initialization Conflict**
- ⚠️ SquareLine calls `HAL_init()` inside `GUI_initHAL()`
- Current main.c likely has its own HAL initialization
- **Action:** Decide:
  - Option A: Keep SquareLine's HAL_init() call (remove from main.c)
  - Option B: Disable it in SquareLine UI and call from main.c
  - Recommend: **Option B** - keep main.c as single source of HAL truth

#### 3. **Critical Timing: 500ms LVGL Delay**
- ⚠️ The initialization sequence MUST be:
  ```
  1. Display hardware reset
  2. NVS init
  3. SD card init
  4. bsp_display_start()
  5. bsp_display_backlight_on()
  6. bsp_display_lock(-1)
  7. GUI_load() ← Call SquareLine UI here
  8. lv_obj_invalidate(lv_scr_act())
  9. bsp_display_unlock()
  10. vTaskDelay(500ms) ← THEN wait 500ms
  11. i2s_init() ← THEN start I2S
  ```
- **Action:** Verify main.c respects this order; GUI_load() must happen before the 500ms delay

#### 4. **Display Resolution Verification**
- ❓ Is the UI designed for 480×800?
- Need to check: SquareLine project resolution setting
- **Action:** Confirm resolution in SquareLine file metadata
  - If wrong: Need to re-export at 480×800

---

### 🟡 IMPORTANT - Review & Fix

#### 5. **Missing Weather Widget Screen?**
- The "home" screen has labels for weather (CURRENTTEMPQ, CURRENTSTATUSQ, 60X60ICONQ)
- But no dedicated weather widget as described in session notes
- **Action:** Confirm if weather display is part of home screen (likely yes)

#### 6. **Event Handler Stubs**
- Found: `GUI_event__Image__home__SETTINGSBUTTON__Clicked()`
- These are declared but may need implementation
- **Action:** Check behavior/events.c - do handlers have real code or are they stubs?

#### 7. **WiFi & Video Player Screens Missing**
- Session notes mention: "WiFi → Video player → Weather widget"
- Currently only have: Settings → ZIP code input
- **Action:** These are Phase 2 tasks - not blocking current integration

---

### 🟢 GOOD - No Action Needed

#### 8. **Asset Embedding** ✅
- All fonts and images are embedded as .c files
- No external file dependencies
- Ready for firmware

#### 9. **CMakeLists.txt** ✅
- Configured as library (`add_library`)
- Can be cleanly integrated into main/CMakeLists.txt

#### 10. **Modular Component System** ✅
- Custom components (background_img_component, comp_hook)
- Clean abstraction for reusable UI elements

---

## Pre-Integration Tasks

### Before Integration: Verify in SquareLine
- [ ] Project resolution is 480×800 (NOT 430×272)
- [ ] Color format is RGB565
- [ ] LVGL version target is 9.4
- [ ] All fonts are rendered correctly
- [ ] Image assets are properly scaled for 480×800 display

### If Any Wrong: Re-Export
```
SquareLine Project Settings:
- Resolution: 480×800
- Color: RGB565
- LVGL: 9.4
- Export as: C code (ui/ folder)
```

---

## Integration Steps (Once Ready)

1. **Copy UI folder**
   ```
   C:\Users\Administrator\Documents\DeskMediaDeviceV2\UI\Tempra Control_UIFiles\
   → main/ui/
   ```

2. **Update main/CMakeLists.txt**
   ```cmake
   # Add at end of file:
   add_subdirectory(ui)
   target_link_libraries(${COMPONENT_LIB} GUI)
   ```

3. **Update main/main.c**
   - Add: `#include "ui/GUI.h"`
   - Replace `create_ui()` with:
     ```c
     void create_ui(void) {
         GUI_load();
     }
     ```
   - Verify 500ms delay is AFTER GUI_load(), BEFORE i2s_init()

4. **Handle HAL init**
   - If HAL_init() is defined in SquareLine: Stub it out
   - Keep main.c HAL initialization

5. **Build**
   ```powershell
   .\preflight_and_clean.ps1
   .\build.ps1
   .\flash.ps1
   # Manual power cycle
   .\monitor.ps1
   ```

---

## Potential Issues During Build

### 1. Linker Error: "Multiple definition of HAL_init"
- **Fix:** Open `ui/core/gui_core.c`, find HAL_init() call, comment it out or stub it

### 2. Display Blank After Flash
- **Cause:** 500ms delay timing or I2S DMA starting too early
- **Fix:** Check app_main() initialization order per CRITICAL TIMING above

### 3. Font/Image Not Found
- **Cause:** Missing includes or asset registration
- **Fix:** Verify all `LV_IMG_DECLARE` and `LV_FONT_DECLARE` in GUI.h match generated files

### 4. Memory Overflow
- **Cause:** Too many fonts/images for available RAM
- **Fix:** Use CONFIG_SPIRAM=y (already set) and optimize asset sizes

---

## Questions for You

1. ✅ Is the UI designed at 480×800 resolution? (Confirm in SquareLine)
2. ❓ Does the home screen have all the weather widget elements?
3. ❓ Are the event handlers (settingsbutton_clicked) implemented?
4. ❓ Do you have the SquareLine project file (.slvs) for future edits?

---

## Summary

| Item | Status | Action |
|------|--------|--------|
| Structure | ✅ Good | None |
| LVGL Version | ❓ Verify | Check sdkconfig |
| Resolution | ❓ Verify | Confirm 480×800 |
| HAL Init | ⚠️ Review | Stub out in UI code |
| 500ms Timing | ⚠️ Critical | Verify app_main() order |
| Events | ❓ Review | Check behavior/events.c |
| Ready to Build | 🟡 Pending | After above verified |

**Next Step:** Answer the 3 verification questions, then we proceed with integration.
