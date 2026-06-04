#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMAGE_UPLOAD_BG2_8B751F9997764EDCA3D7F03DB3DD4A54_PNG
#define LV_ATTRIBUTE_IMAGE_UPLOAD_BG2_8B751F9997764EDCA3D7F03DB3DD4A54_PNG
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMAGE_UPLOAD_BG2_8B751F9997764EDCA3D7F03DB3DD4A54_PNG uint8_t upload_bg2_8b751f9997764edca3d7f03db3dd4a54_png_map[] = {
  0x86, 0x31, 
};

const lv_image_dsc_t upload_bg2_8b751f9997764edca3d7f03db3dd4a54_png = {
  .header.cf = LV_COLOR_FORMAT_RGB565,
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.w = 1,
  .header.h = 1,
  .data_size = 1 * 2,
  .data = upload_bg2_8b751f9997764edca3d7f03db3dd4a54_png_map,
};
