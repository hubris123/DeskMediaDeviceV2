#pragma once

/**
 * Weather icon lookup for Open-Meteo WMO codes → SquareLine image descriptors.
 *
 * Icons are embedded as lv_image_dsc_t arrays (no SD card, no PNG decoder needed).
 * All variable names extracted from SquareLine's GUI.h and assets/images/ .c export.
 *
 * Supported WMO codes (actual Open-Meteo set — codes 611-616 do NOT exist):
 *   0, 1, 2, 3, 45, 48, 51-57, 61-67, 71-77, 80-82, 85-86, 95-99
 *
 * Icon sets:
 *   60x60 — main weather icon on home screen  (GUI_Image__home__60X60ICONQ)
 *   22x22 — hourly forecast icons             (GUI_Image__home__HR1-5FORCASICON22X22Q)
 */

#include "lvgl.h"

/* ── 60x60 icon declarations ─────────────────────────────────────────── */
LV_IMG_DECLARE(upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png);   // clear day
LV_IMG_DECLARE(upload_2_e2278bcef5404f3da02d7aa2a510f534_png);   // mainly clear day
LV_IMG_DECLARE(upload_3_20748ad6e84045c396cda8b366aa5d07_png);   // partly cloudy day
LV_IMG_DECLARE(upload_7_5e6957ba671848fcae58518a4764438c_png);   // cloudy (day+night)
LV_IMG_DECLARE(upload_11_d928da2afce943088fc003fded319081_png);  // fog    (day+night)
LV_IMG_DECLARE(upload_12_71141d8aba63422c8a7377026b8cecc6_png);  // drizzle (day+night)
LV_IMG_DECLARE(upload_14_14ff6d55212c4e7b945e173b90b81139_png);  // rain showers (day+night)
LV_IMG_DECLARE(upload_15_3a414dd3d7e14d559033812d417eaa90_png);  // thunderstorm (day+night)
LV_IMG_DECLARE(upload_18_b42c444b7f1445fc9003e4caa65d5a60_png);  // rain   (day+night)
LV_IMG_DECLARE(upload_19_2425f3503fc04098b87dd50fc6604b61_png);  // snow showers (day+night)
LV_IMG_DECLARE(upload_22_6dccc7824e764e4a9b70ff8d5b078fad_png);  // snow   (day+night)
LV_IMG_DECLARE(upload_26_28f0c2296c7742bbad7e6ed91f80b81a_png);  // freezing rain (day+night)
LV_IMG_DECLARE(upload_33_b02a9c2486c94766bd9856f56db21f1f_png);  // clear night
LV_IMG_DECLARE(upload_34_4c27b41ae3a1496089c4d14141615cbd_png);  // mainly clear night
LV_IMG_DECLARE(upload_35_cc7cb52862b9422aba85d07b935bc63c_png);  // partly cloudy night

/* ── 22x22 icon declarations ─────────────────────────────────────────── */
LV_IMG_DECLARE(upload_1s_a942bd7a65904cab870f8cf337226275_png);
LV_IMG_DECLARE(upload_2s_37e4aa2fa7024542b513edb68eebbfa3_png);
LV_IMG_DECLARE(upload_3s_e6f8aad40a204657b22d932c4b20fb18_png);
LV_IMG_DECLARE(upload_7s_c385fe6874fa4da5853f367e29db7db4_png);
LV_IMG_DECLARE(upload_11s_7c1fa968af52485bbeb874d89e56f96b_png);
LV_IMG_DECLARE(upload_12s_7ed83dc3cb5f41f1a541dfd52ba15f5c_png);
LV_IMG_DECLARE(upload_14s_b570241f67234745a56f9ddc06bd7800_png);
LV_IMG_DECLARE(upload_15s_71a05d920e1a428e86707846a7f46127_png);
LV_IMG_DECLARE(upload_18s_e55d60ec77034c32846327663b493f72_png);
LV_IMG_DECLARE(upload_19s_c04d370ce95c403b82416b454729a8a8_png);
LV_IMG_DECLARE(upload_22s_f739a4fff8ce46c5a09433c09b5b94c6_png);
LV_IMG_DECLARE(upload_26s_731f337d30e945da969f8e1271adfa24_png);
LV_IMG_DECLARE(upload_33s_b5e096b136ce4b6fbf7748e265b3d5eb_png);
LV_IMG_DECLARE(upload_34s_d824cbffe8e8400cabc43e41608fd72d_png);
LV_IMG_DECLARE(upload_35s_7a2b8dbb6e8a4c1d8a796fed8458d198_png);

/**
 * Get the 60x60 weather icon for a given WMO code.
 * @param wmo_code  WMO weather code from Open-Meteo
 * @param is_night  true after sunset, false during day
 * @return pointer to lv_image_dsc_t (never NULL — defaults to clear day)
 */
const lv_image_dsc_t *weather_icon_60(int wmo_code, bool is_night);

/**
 * Get the 22x22 weather icon for a given WMO code.
 * @param wmo_code  WMO weather code from Open-Meteo
 * @param is_night  true after sunset, false during day
 * @return pointer to lv_image_dsc_t (never NULL)
 */
const lv_image_dsc_t *weather_icon_22(int wmo_code, bool is_night);

/**
 * Get a short human-readable status string for a WMO code.
 * Suitable for GUI_Label__home__CURRENTSTATUSQ.
 */
const char *weather_description(int wmo_code);
