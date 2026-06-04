/*******************************************************************************
 * Size: 7 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

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

#ifndef TINY
#define TINY 1
#endif

#if TINY

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x9a, 0x89, 0x78, 0x44, 0x88,

    /* U+0022 "\"" */
    0xa7, 0xb9, 0x6a, 0x0, 0x0,

    /* U+0023 "#" */
    0x8, 0x3b, 0x7, 0xfc, 0xfb, 0xc, 0xc, 0xa,
    0xfc, 0xe8, 0xc, 0x39, 0x0,

    /* U+0024 "$" */
    0x2, 0xb0, 0x5, 0xee, 0xd0, 0xaa, 0xa0, 0x2,
    0xbf, 0xb0, 0x21, 0xbf, 0x37, 0xef, 0xa0, 0x0,
    0x80, 0x0,

    /* U+0025 "%" */
    0x7b, 0x41, 0xb0, 0xb2, 0x9a, 0x20, 0x5a, 0x89,
    0xa6, 0x1, 0xb7, 0x4b, 0xa, 0x23, 0xa9,

    /* U+0026 "&" */
    0xb, 0xd7, 0x1, 0xe7, 0x90, 0x2d, 0xf4, 0x5a,
    0x74, 0xf8, 0x4e, 0xea, 0xa0, 0x0, 0x0,

    /* U+0027 "'" */
    0xa4, 0x93, 0x0,

    /* U+0028 "(" */
    0xd, 0x34, 0xc0, 0x6a, 0x6, 0xa0, 0x4c, 0x0,
    0xd3,

    /* U+0029 ")" */
    0xb5, 0x4c, 0x2e, 0x2e, 0x4c, 0xb5,

    /* U+002A "*" */
    0x5b, 0x59, 0xf9, 0x39, 0x30,

    /* U+002B "+" */
    0x2, 0x30, 0x5, 0x80, 0x7e, 0xfa, 0x5, 0x80,

    /* U+002C "," */
    0x32, 0x98, 0x83,

    /* U+002D "-" */
    0x9e, 0x40,

    /* U+002E "." */
    0x32, 0x97,

    /* U+002F "/" */
    0x0, 0x1d, 0x0, 0x6, 0x80, 0x0, 0xc3, 0x0,
    0x1d, 0x0, 0x6, 0x80, 0x0, 0xb3, 0x0, 0x1e,
    0x0, 0x0,

    /* U+0030 "0" */
    0x1c, 0xf9, 0x9, 0xa1, 0xd4, 0xb7, 0xb, 0x79,
    0xa1, 0xd4, 0x1c, 0xf9, 0x0,

    /* U+0031 "1" */
    0xef, 0x20, 0xf2, 0xf, 0x20, 0xf2, 0xf, 0x20,

    /* U+0032 "2" */
    0x7e, 0xe4, 0x31, 0x8b, 0x1, 0xd5, 0x1d, 0x60,
    0xbf, 0xfe,

    /* U+0033 "3" */
    0xbf, 0xfa, 0x3, 0xd1, 0x8, 0xe7, 0x30, 0x6d,
    0xaf, 0xe5,

    /* U+0034 "4" */
    0x0, 0xc5, 0x0, 0x8a, 0x0, 0x4e, 0x2e, 0xc,
    0xff, 0xfb, 0x0, 0x2f, 0x0,

    /* U+0035 "5" */
    0x5f, 0xfa, 0x79, 0x0, 0x8f, 0xe7, 0x20, 0x6e,
    0x9f, 0xe6,

    /* U+0036 "6" */
    0xa, 0xfd, 0x8, 0xc0, 0x10, 0xbd, 0xea, 0x9,
    0xb0, 0xe3, 0x1c, 0xea, 0x0,

    /* U+0037 "7" */
    0xdf, 0xff, 0x1a, 0x26, 0xc0, 0x0, 0xd5, 0x0,
    0x4e, 0x0, 0xb, 0x70, 0x0,

    /* U+0038 "8" */
    0x2d, 0xe9, 0x8, 0xa1, 0xf2, 0x4f, 0xed, 0xa,
    0x80, 0xe4, 0x4d, 0xeb, 0x10,

    /* U+0039 "9" */
    0x5e, 0xe6, 0xc, 0x63, 0xf0, 0x5d, 0xcf, 0x20,
    0x5, 0xf0, 0x6f, 0xd4, 0x0,

    /* U+003A ":" */
    0x97, 0x32, 0x32, 0x97,

    /* U+003B ";" */
    0x97, 0x32, 0x32, 0x98, 0x83,

    /* U+003C "<" */
    0x0, 0x2, 0x17, 0xd8, 0x8e, 0x30, 0x5, 0xb9,
    0x0, 0x1,

    /* U+003D "=" */
    0x8e, 0xea, 0x0, 0x0, 0x7e, 0xea,

    /* U+003E ">" */
    0x20, 0x0, 0x5d, 0x82, 0x1, 0xcc, 0x7c, 0x61,
    0x10, 0x0,

    /* U+003F "?" */
    0x7e, 0xe5, 0x31, 0x8a, 0x3, 0xd2, 0x3, 0x40,
    0x7, 0x90,

    /* U+0040 "@" */
    0x5, 0xba, 0xb7, 0x4, 0xa8, 0xdc, 0xb8, 0xa4,
    0xe0, 0xb6, 0xba, 0x4d, 0xa, 0x6a, 0x4b, 0xac,
    0xad, 0x70, 0x5b, 0xba, 0x10,

    /* U+0041 "A" */
    0x0, 0x1f, 0x70, 0x0, 0x8, 0xbe, 0x0, 0x0,
    0xe1, 0xc5, 0x0, 0x6f, 0xef, 0xc0, 0xd, 0x40,
    0xe, 0x30,

    /* U+0042 "B" */
    0x6f, 0xee, 0x60, 0x6b, 0x7, 0xc0, 0x6f, 0xdf,
    0x70, 0x6b, 0x3, 0xf0, 0x6f, 0xee, 0x90,

    /* U+0043 "C" */
    0x8, 0xfe, 0x67, 0xd2, 0x24, 0xb7, 0x0, 0x7,
    0xd2, 0x24, 0x8, 0xfe, 0x60,

    /* U+0044 "D" */
    0x6f, 0xfe, 0x70, 0x6b, 0x2, 0xe4, 0x6b, 0x0,
    0xa7, 0x6b, 0x2, 0xe4, 0x6f, 0xfe, 0x70,

    /* U+0045 "E" */
    0x6f, 0xff, 0x46, 0xb0, 0x0, 0x6f, 0xee, 0x6,
    0xb0, 0x0, 0x6f, 0xff, 0x50,

    /* U+0046 "F" */
    0x6f, 0xff, 0x46, 0xb0, 0x0, 0x6f, 0xfe, 0x6,
    0xb0, 0x0, 0x6b, 0x0, 0x0,

    /* U+0047 "G" */
    0x8, 0xfe, 0x70, 0x7d, 0x21, 0x40, 0xb7, 0x1,
    0x80, 0x7d, 0x23, 0xe0, 0x8, 0xfe, 0x80,

    /* U+0048 "H" */
    0x6b, 0x1, 0xf1, 0x6b, 0x1, 0xf1, 0x6f, 0xff,
    0xf1, 0x6b, 0x1, 0xf1, 0x6b, 0x1, 0xf1,

    /* U+0049 "I" */
    0x6b, 0x6b, 0x6b, 0x6b, 0x6b,

    /* U+004A "J" */
    0x9, 0xff, 0x40, 0x0, 0xe4, 0x0, 0xe, 0x40,
    0x41, 0xf3, 0xb, 0xfb, 0x0,

    /* U+004B "K" */
    0x6b, 0x9, 0xa0, 0x6b, 0x8b, 0x0, 0x6f, 0xf5,
    0x0, 0x6d, 0x5e, 0x20, 0x6b, 0x7, 0xd0,

    /* U+004C "L" */
    0x6b, 0x0, 0x6, 0xb0, 0x0, 0x6b, 0x0, 0x6,
    0xb0, 0x0, 0x6f, 0xff, 0x20,

    /* U+004D "M" */
    0x6c, 0x0, 0x2f, 0x16, 0xf6, 0xb, 0xf1, 0x6c,
    0xe5, 0xbf, 0x16, 0xa8, 0xf2, 0xf1, 0x6a, 0x5,
    0xf, 0x10,

    /* U+004E "N" */
    0x6d, 0x0, 0xf1, 0x6f, 0xb0, 0xf1, 0x6b, 0xd9,
    0xf1, 0x6b, 0x2e, 0xf1, 0x6b, 0x4, 0xf1,

    /* U+004F "O" */
    0x8, 0xfe, 0x80, 0x7d, 0x12, 0xe6, 0xb7, 0x0,
    0x99, 0x7d, 0x22, 0xe6, 0x8, 0xfe, 0x80,

    /* U+0050 "P" */
    0x6f, 0xfc, 0x26, 0xb0, 0x7b, 0x6b, 0x8, 0xb6,
    0xff, 0xc2, 0x6b, 0x0, 0x0,

    /* U+0051 "Q" */
    0x8, 0xfe, 0x80, 0x7c, 0x12, 0xd6, 0xb7, 0x0,
    0x99, 0x7d, 0x22, 0xe5, 0x7, 0xef, 0x61, 0x0,
    0xa, 0xd8,

    /* U+0052 "R" */
    0x6f, 0xfc, 0x26, 0xb0, 0x7b, 0x6b, 0x7, 0xb6,
    0xff, 0xf3, 0x6b, 0xb, 0x70,

    /* U+0053 "S" */
    0x3d, 0xfb, 0xa, 0x90, 0x20, 0x3c, 0xe8, 0x2,
    0x11, 0xf2, 0x7e, 0xea, 0x0,

    /* U+0054 "T" */
    0xff, 0xff, 0x40, 0x6b, 0x0, 0x6, 0xb0, 0x0,
    0x6b, 0x0, 0x6, 0xb0, 0x0,

    /* U+0055 "U" */
    0x7a, 0x2, 0xf7, 0xa0, 0x2f, 0x7a, 0x2, 0xf5,
    0xd1, 0x6d, 0xa, 0xfd, 0x40,

    /* U+0056 "V" */
    0xd, 0x50, 0x1f, 0x10, 0x6c, 0x7, 0xa0, 0x0,
    0xf3, 0xe3, 0x0, 0x9, 0xdc, 0x0, 0x0, 0x2f,
    0x60, 0x0,

    /* U+0057 "W" */
    0xa7, 0x9, 0xc0, 0x4d, 0x5c, 0xe, 0xf1, 0x98,
    0x1f, 0x5c, 0xa6, 0xe2, 0xb, 0xe7, 0x4e, 0xd0,
    0x6, 0xf2, 0xf, 0x80,

    /* U+0058 "X" */
    0x8b, 0xb, 0x80, 0xdb, 0xc0, 0x5, 0xf5, 0x0,
    0xda, 0xd0, 0xaa, 0xa, 0xa0,

    /* U+0059 "Y" */
    0xc, 0x60, 0x98, 0x3, 0xe3, 0xe0, 0x0, 0xaf,
    0x50, 0x0, 0x3f, 0x0, 0x0, 0x3f, 0x0,

    /* U+005A "Z" */
    0xaf, 0xff, 0x60, 0x7, 0xc0, 0x5, 0xe1, 0x2,
    0xe3, 0x0, 0xbf, 0xff, 0x80,

    /* U+005B "[" */
    0x6f, 0x66, 0xa0, 0x6a, 0x6, 0xa0, 0x6a, 0x6,
    0xf6,

    /* U+005C "\\" */
    0x2d, 0x0, 0xc, 0x20, 0x7, 0x70, 0x2, 0xd0,
    0x0, 0xc2, 0x0, 0x77, 0x0, 0x2d,

    /* U+005D "]" */
    0xcf, 0x1f, 0x1f, 0x1f, 0x1f, 0xcf,

    /* U+005E "^" */
    0x8, 0xb0, 0xc, 0xa2, 0x66, 0x39,

    /* U+005F "_" */
    0xaa, 0xa5,

    /* U+0060 "`" */
    0xa, 0x40,

    /* U+0061 "a" */
    0x5e, 0xe5, 0x21, 0x6c, 0x5b, 0xbd, 0x7c, 0xad,

    /* U+0062 "b" */
    0x89, 0x0, 0x8, 0xde, 0xc1, 0x8b, 0xa, 0x78,
    0xb0, 0xa7, 0x8d, 0xec, 0x10,

    /* U+0063 "c" */
    0x2c, 0xf8, 0xb, 0x80, 0x40, 0xb8, 0x4, 0x2,
    0xcf, 0x80,

    /* U+0064 "d" */
    0x0, 0xc, 0x52, 0xde, 0xe5, 0xa8, 0xd, 0x5a,
    0x70, 0xd5, 0x2d, 0xde, 0x50,

    /* U+0065 "e" */
    0x2c, 0xd9, 0xb, 0xca, 0xd1, 0xb9, 0x3, 0x2,
    0xce, 0x90,

    /* U+0066 "f" */
    0x3e, 0xad, 0xf9, 0x5b, 0x5, 0xb0, 0x5b, 0x0,

    /* U+0067 "g" */
    0x4d, 0xed, 0x6b, 0x80, 0xd6, 0x4d, 0xee, 0x62,
    0x20, 0xd5, 0x4e, 0xea, 0x0,

    /* U+0068 "h" */
    0x89, 0x0, 0x8, 0xde, 0xc0, 0x8c, 0x1d, 0x58,
    0x90, 0xb5, 0x89, 0xb, 0x50,

    /* U+0069 "i" */
    0x79, 0x9b, 0x89, 0x89, 0x89,

    /* U+006A "j" */
    0x6, 0xa0, 0x8c, 0x7, 0xa0, 0x7a, 0x7, 0xa7,
    0xe5,

    /* U+006B "k" */
    0x89, 0x0, 0x8, 0x95, 0xe2, 0x8d, 0xf2, 0x8,
    0xed, 0x80, 0x89, 0x2e, 0x40,

    /* U+006C "l" */
    0x89, 0x89, 0x89, 0x89, 0x89,

    /* U+006D "m" */
    0x8d, 0xeb, 0xbf, 0x68, 0xb0, 0xf6, 0x5d, 0x89,
    0xd, 0x33, 0xd8, 0x90, 0xd3, 0x3d,

    /* U+006E "n" */
    0x8d, 0xec, 0x8, 0xc0, 0xd5, 0x89, 0xb, 0x58,
    0x90, 0xb5,

    /* U+006F "o" */
    0x2d, 0xf8, 0xb, 0x70, 0xe4, 0xb7, 0xe, 0x42,
    0xce, 0x90,

    /* U+0070 "p" */
    0x8d, 0xdc, 0x18, 0xb0, 0xa7, 0x8b, 0xa, 0x78,
    0xde, 0xc1, 0x89, 0x0, 0x0,

    /* U+0071 "q" */
    0x2d, 0xee, 0x5a, 0x70, 0xe5, 0xa7, 0xe, 0x52,
    0xde, 0xe5, 0x0, 0xc, 0x50,

    /* U+0072 "r" */
    0x8d, 0xc8, 0xc1, 0x89, 0x8, 0x90,

    /* U+0073 "s" */
    0x5d, 0xe3, 0xaa, 0x40, 0x25, 0xc6, 0x9d, 0xd3,

    /* U+0074 "t" */
    0x5b, 0xd, 0xf9, 0x5b, 0x5, 0xc0, 0x1d, 0xb0,

    /* U+0075 "u" */
    0x89, 0xc, 0x48, 0x90, 0xc4, 0x7b, 0xe, 0x42,
    0xde, 0xe4,

    /* U+0076 "v" */
    0xd, 0x40, 0xe1, 0x7, 0xa6, 0xa0, 0x1, 0xfd,
    0x30, 0x0, 0xad, 0x0,

    /* U+0077 "w" */
    0xd3, 0x5e, 0x9, 0x57, 0x8a, 0xe4, 0xe0, 0x2d,
    0xe6, 0xdb, 0x0, 0xd9, 0xf, 0x50,

    /* U+0078 "x" */
    0x9a, 0x6b, 0x0, 0xde, 0x10, 0xd, 0xe2, 0x9,
    0x97, 0xc0,

    /* U+0079 "y" */
    0xd, 0x41, 0xe0, 0x6, 0xb7, 0x90, 0x0, 0xee,
    0x20, 0x0, 0x8b, 0x0, 0xc, 0xd2, 0x0,

    /* U+007A "z" */
    0x9e, 0xf8, 0x5, 0xd0, 0x2e, 0x20, 0xbf, 0xe8,

    /* U+007B "{" */
    0xd, 0x91, 0xf0, 0x9d, 0x1, 0xf0, 0x1f, 0x0,
    0xc9,

    /* U+007C "|" */
    0x69, 0x69, 0x69, 0x69, 0x69, 0x69,

    /* U+007D "}" */
    0xd9, 0x4, 0xd0, 0x2f, 0x54, 0xd0, 0x4d, 0xd,
    0x80,

    /* U+007E "~" */
    0x4c, 0x4a, 0x82, 0xb6
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 32, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 32, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 49, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 10, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 71, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 41, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 82, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 71, .adv_w = 26, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 74, .adv_w = 40, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 83, .adv_w = 40, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 89, .adv_w = 49, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 94, .adv_w = 67, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 102, .adv_w = 29, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 105, .adv_w = 43, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 107, .adv_w = 29, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 109, .adv_w = 44, .box_w = 5, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 127, .adv_w = 76, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 44, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 66, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 66, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 67, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 69, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 29, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 29, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 252, .adv_w = 67, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 67, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 268, .adv_w = 67, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 66, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 116, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 309, .adv_w = 86, .box_w = 7, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 86, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 82, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 93, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 370, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 72, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 396, .adv_w = 86, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 90, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 426, .adv_w = 37, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 61, .box_w = 5, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 444, .adv_w = 83, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 459, .adv_w = 68, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 472, .adv_w = 107, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 90, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 505, .adv_w = 95, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 82, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 95, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 551, .adv_w = 82, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 69, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 88, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 84, .box_w = 7, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 130, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 641, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 76, .box_w = 6, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 669, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 682, .adv_w = 41, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 691, .adv_w = 44, .box_w = 4, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 705, .adv_w = 41, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 711, .adv_w = 67, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 717, .adv_w = 56, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 719, .adv_w = 67, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 721, .adv_w = 69, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 66, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 752, .adv_w = 78, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 765, .adv_w = 71, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 43, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 783, .adv_w = 78, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 796, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 809, .adv_w = 34, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 814, .adv_w = 34, .box_w = 3, .box_h = 6, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 823, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 34, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 841, .adv_w = 117, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 855, .adv_w = 77, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 73, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 875, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 888, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 901, .adv_w = 48, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 907, .adv_w = 59, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 915, .adv_w = 49, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 923, .adv_w = 77, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 933, .adv_w = 67, .box_w = 6, .box_h = 4, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 945, .adv_w = 105, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 959, .adv_w = 67, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 969, .adv_w = 67, .box_w = 6, .box_h = 5, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 984, .adv_w = 61, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 992, .adv_w = 44, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1001, .adv_w = 35, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1007, .adv_w = 44, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1016, .adv_w = 67, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 0, 13, 14, 15, 16, 17,
    18, 19, 12, 20, 20, 0, 0, 0,
    21, 22, 23, 24, 25, 22, 26, 27,
    28, 29, 29, 30, 31, 32, 29, 29,
    22, 33, 34, 35, 3, 36, 30, 37,
    37, 38, 39, 40, 41, 42, 43, 0,
    44, 0, 45, 46, 47, 48, 49, 50,
    51, 45, 52, 52, 53, 48, 45, 45,
    46, 46, 54, 55, 56, 57, 51, 58,
    58, 59, 58, 60, 41, 0, 0, 9
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 12,
    18, 19, 20, 21, 21, 0, 0, 0,
    22, 23, 24, 25, 23, 25, 25, 25,
    23, 25, 25, 26, 25, 25, 25, 25,
    23, 25, 23, 25, 3, 27, 28, 29,
    29, 30, 31, 32, 33, 34, 35, 0,
    36, 0, 37, 38, 39, 39, 39, 0,
    39, 38, 40, 41, 38, 38, 42, 42,
    39, 42, 39, 42, 43, 44, 45, 46,
    46, 47, 46, 48, 0, 0, 35, 9
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 5, 0, 3, -2, 0, 0, 0,
    0, -6, -7, 0, 5, 2, 2, -4,
    0, 4, 0, 4, 1, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, 0, 0, 0, 0, -2,
    2, 2, 0, 0, -1, 0, 0, 1,
    0, -1, 0, -1, -1, -2, 0, 0,
    0, 0, -1, 0, 0, -1, -2, 0,
    0, -1, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, -3, 0, -14, 0, 0, -2, 0,
    2, 3, 0, 0, -2, 1, 1, 4,
    2, -2, 2, 0, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -4, 0, -4, 0, 0, 0, 0,
    0, 0, 3, 0, -3, -1, -1, 1,
    0, -2, 0, 0, -1, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, -1, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 1, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 2, 1, 3, -1, 0, 0, 2,
    -1, -4, -15, 0, 3, 2, 0, -1,
    0, 3, 0, 3, 0, 3, 0, -11,
    0, -1, 3, 0, 4, -1, 2, 1,
    0, 0, 1, -1, 0, 0, -2, 9,
    0, 9, 0, 3, 0, 4, 2, 2,
    0, 0, 0, -4, 0, 0, 0, 0,
    1, -1, 0, 0, -2, -2, -2, 0,
    0, -1, 0, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, -6, 0, -7, 0, 0, 0, 0,
    -1, 0, 10, -1, -2, 1, 1, 0,
    0, -2, 1, 0, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 0, 0, -4, 0, 4, 0,
    -8, -11, -8, -2, 3, 0, 0, -7,
    0, 1, -3, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 3, -13, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -2, 0, -1, -1, -1, 0,
    0, 0, 0, 0, 0, -2, 0, -1,
    0, -3, -2, 0, -3, -4, -4, -2,
    0, -2, 0, -2, 0, 0, 0, 0,
    -1, 0, 0, 1, 0, 0, -1, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 2, 0, 0, -1, 0, -2, 0,
    0, 0, 0, 3, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, -1, -2, 0,
    0, -3, 0, -3, 2, 0, 0, -2,
    1, 2, 3, 0, -3, 0, -1, 0,
    0, -5, 1, 0, 1, -6, 1, 0,
    0, 1, -6, 0, -6, -1, -10, 0,
    0, -6, 0, 2, 3, 0, 2, 0,
    0, 0, 0, 0, 0, -3, -2, 0,
    0, 0, 0, -1, 0, 0, 0, -1,
    0, 0, 0, 0, 0, -1, -1, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, -1, -1, -1, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, 0, 0, -2, 1, 0, 0, -1,
    1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    -1, 0, 0, 0, 0, -2, -2, 0,
    0, 3, 0, 1, -3, 0, 0, 3,
    -6, -5, -4, -2, 1, 0, -1, -7,
    -2, 0, -2, 0, -2, 2, -2, -7,
    0, -3, 0, 0, 1, 0, 1, 0,
    0, 1, 1, -3, -4, 0, -6, -2,
    -2, -2, -3, -1, -3, 0, -1, -2,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, -1,
    0, 0, -1, 0, -2, -3, -3, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, -1, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, -6, -3, 0, 0, 0, -2, -6,
    0, 0, -1, 1, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 1, 0,
    0, -2, -2, 0, -1, -1, -1, 0,
    0, 0, 0, 0, 0, -3, 0, -1,
    0, -2, -1, 0, -3, -3, -3, -1,
    0, -2, 1, -3, 0, 0, 0, 0,
    9, 0, 0, 1, 0, 0, -2, 0,
    0, -5, 0, 0, 0, 0, 0, -11,
    -3, 4, 3, -1, -5, 0, 1, -2,
    0, -6, -1, -1, 1, -8, -1, 3,
    0, 2, -4, -2, -4, -4, -5, 0,
    0, -7, 0, 6, 0, 0, -1, 0,
    0, 0, -1, -1, -1, -3, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, -1, -2, 0,
    0, -2, 0, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -2, 0, 0, 2,
    -1, 1, 0, -2, 1, 0, 0, -2,
    -1, 0, -1, -1, -1, 0, -2, -2,
    0, 0, -1, -1, 0, -2, -1, 0,
    0, -1, 0, 1, 0, 0, -2, 0,
    0, 0, -2, 0, -2, 0, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 1, 0, -1, 0, 0, -1, -2,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, -1, -1, -1, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, -1, 0, -1, -1, -1,
    1, 4, 0, 0, -2, 0, 0, 2,
    0, -1, -4, -1, 2, 1, 0, -5,
    -2, 1, -2, 1, 0, 0, -1, -3,
    0, -2, 1, 0, 0, -2, 0, 0,
    0, 1, 1, -2, -2, 0, -2, -1,
    -2, -1, -1, 0, -2, 1, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, -1, 0, 0, -1, -1, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, -2, 0, 0, 0,
    -4, 0, 0, -3, 2, 0, 0, -5,
    0, 0, -3, -1, 0, -4, -3, -3,
    0, 0, -5, -1, -4, -5, -6, 0,
    -2, 0, 1, 7, -1, 0, -2, -1,
    0, -1, -2, -3, -2, -4, -5, -3,
    0, 0, 0, 0, 1, 0, 0, -8,
    0, 3, 2, -2, -4, 0, 1, -2,
    0, -6, 0, -1, 2, -10, -1, 1,
    0, 0, -7, -1, -5, -1, -8, 0,
    0, -8, 0, 6, 1, 0, 0, 0,
    0, 0, 0, -1, 0, -4, 0, 0,
    0, 0, 0, 0, -3, 0, -1, 0,
    -1, -3, -5, 0, 0, -1, -2, -3,
    -1, 0, -1, 0, 0, 0, 0, -5,
    -1, -4, -3, -1, -2, -3, -1, -2,
    0, -2, -1, -4, -2, 0, -1, -1,
    -1, -1, 0, 1, 0, 0, -4, 0,
    0, -2, 0, 0, 0, 0, 1, 0,
    0, -2, 6, 0, -1, -1, -1, 0,
    0, 0, 0, 0, 0, -3, 0, -1,
    0, -2, -1, 0, -3, -3, -3, -1,
    0, -2, 1, 4, 0, 0, 0, 0,
    9, 0, 0, 1, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, -1, -1, 0, 0, -2, -1, 0,
    0, -2, 0, 2, -1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    2, 1, -1, 0, -3, -1, 0, 3,
    -4, -4, -2, -2, 4, 3, 1, -9,
    -1, 2, -1, 0, -1, 2, -1, -4,
    0, -1, 1, -1, -1, -3, -1, 0,
    0, 3, 2, 0, -3, 0, -6, -1,
    5, -1, -4, 1, -1, -4, -4, -1,
    1, 0, -2, 0, -3, 0, 1, 4,
    -3, -4, -4, -3, 3, 0, 1, -9,
    -1, 1, -2, -1, -3, 0, -3, -4,
    -2, -2, -1, 0, 0, -3, -3, -1,
    0, 3, 3, -1, -6, 0, -6, -1,
    0, -4, -7, -1, -4, -2, -4, -3,
    0, 0, -1, 0, -2, -1, 0, -1,
    -2, 0, 2, -4, 1, 0, 0, -6,
    0, -1, -3, -2, -1, -3, -3, -4,
    -2, 0, -3, -1, -3, -1, -3, -1,
    0, 0, 1, 5, -2, 0, -3, -1,
    0, -1, -2, -3, -3, -4, -5, -2,
    2, 0, -2, 0, -6, -1, 1, 2,
    -4, -4, -2, -4, 4, -1, 1, -11,
    -2, 2, -3, -2, -4, 0, -3, -5,
    -1, -1, -1, -1, -3, -3, -1, 0,
    0, 3, 4, 0, -7, 0, -7, -1,
    4, -4, -8, -2, -4, -5, -6, -4,
    0, 0, 0, 0, -1, 0, 0, 1,
    -1, 2, 0, -2, 2, 0, 0, -2,
    0, 0, 0, 0, 1, 1, -1, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 1, 3, 0, 0, -1, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    1, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 5, 0, 3, 1, 1, -2,
    0, 2, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, -1, 2, 0, 3, 0,
    0, 10, 1, -2, -2, 1, 1, 0,
    1, -6, 0, 0, 6, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 5, 16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, 0, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 1, 0,
    0, 1, 13, -2, -1, 3, 3, -3,
    1, 0, 0, 1, 1, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -15, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, -3,
    0, 0, 0, 0, -2, -1, 0, 0,
    0, -2, 0, -1, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, -2, 0, -3, 0, 0, 0, -2,
    1, -1, 0, 0, -3, -1, -2, 0,
    0, -3, 0, -1, 0, -5, 0, -2,
    0, 0, -7, -1, -4, -2, -4, 0,
    0, -7, 0, -3, -1, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -2, -1,
    0, 0, 0, 0, -2, 0, -3, 2,
    -2, 2, 0, 0, -2, 0, -2, -1,
    0, -1, -1, 0, 1, -3, -1, 0,
    0, 0, -8, -1, -3, 0, -4, 0,
    0, -5, -1, 0, 0, 0, -1, 0,
    0, 0, 0, 1, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, -2,
    1, 0, 0, 0, -3, -1, -2, 0,
    0, -3, 0, -1, 0, -5, 0, 0,
    0, 0, -11, 0, -2, -4, -6, 0,
    0, -7, 0, -1, -2, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -2, -1,
    1, 0, 0, 2, -2, 0, 4, 4,
    -1, -1, -3, 1, 4, 2, 2, -3,
    1, 4, 1, 3, 2, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 5, -3, -1, 0, -1, 9,
    5, 9, 0, 0, 0, 1, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, -7, -1, -1, -4, -4, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, -7, -1, -1, -4, -4, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    -3, 1, 0, -1, 1, 3, 1, -3,
    0, 0, -1, 1, 0, 1, 0, 0,
    0, 0, -1, 0, -1, 0, -2, 0,
    -1, -4, 0, 6, -1, 0, -3, 0,
    0, 0, -2, 0, -1, -3, -2, -1,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, -7, -1, -1, -4, -4, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -3, -1, -1, 3,
    -1, -1, -4, 1, 1, 1, 0, -2,
    0, 3, 0, 1, 1, 1, -2, -5,
    -1, 0, -2, -1, -3, -4, -4, 0,
    -1, -2, -1, -2, -1, 0, -1, 0,
    0, 0, 0, 2, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, -3, 0, -1, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, 0, 0, 0, -1, 0, 0, -2,
    -1, 1, 0, -2, -2, 0, 0, -2,
    0, -2, -1, -1, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 3, 0, 0, -2, 0,
    0, 0, 0, -1, 0, -1, 0, 0,
    0, 0, 0, 0, -3, 0, 0, 4,
    -2, -4, -4, 0, 2, 2, 0, -4,
    0, 2, 0, 3, 0, 4, 0, -3,
    0, 0, -2, 0, 0, -3, -3, 0,
    0, -2, 0, -2, -2, 0, -2, 0,
    -2, 0, -1, 2, 0, -1, -3, -1,
    0, 0, -1, 0, -2, 0, 0, 2,
    -3, 0, 1, -1, 1, 1, 0, -4,
    0, 0, -1, 0, -1, 2, -1, 0,
    0, 0, -2, -1, -3, 0, -3, 0,
    0, -5, 0, 4, -1, 0, -2, 0,
    3, 0, -1, 0, -1, -3, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 1,
    -1, 1, 0, 0, -1, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, 2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, -1, -1, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 60,
    .right_class_cnt     = 48,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t tiny = {
#else
lv_font_t tiny = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 7,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if TINY*/

