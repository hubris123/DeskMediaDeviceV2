#include "../../GUI.h"

void GUI_initGlobalStyles() {
  lv_style_init(&GUI_Style__class_8rQJj2WR7KS5BW__);
  lv_style_set_pad_all(&GUI_Style__class_8rQJj2WR7KS5BW__, 28);
  lv_style_set_bg_color(&GUI_Style__class_8rQJj2WR7KS5BW__,
                        lv_color_make(95, 95, 95));
  lv_style_set_bg_opa(&GUI_Style__class_8rQJj2WR7KS5BW__, 255);
  lv_style_set_radius(&GUI_Style__class_8rQJj2WR7KS5BW__, 0);

  lv_style_init(&GUI_Style__class_qx6LXcI5pACvOu__);
  lv_style_set_min_width(&GUI_Style__class_qx6LXcI5pACvOu__, 0);
  lv_style_set_max_width(&GUI_Style__class_qx6LXcI5pACvOu__, 8000);
  lv_style_set_min_height(&GUI_Style__class_qx6LXcI5pACvOu__, 0);
  lv_style_set_max_height(&GUI_Style__class_qx6LXcI5pACvOu__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_qx6LXcI5pACvOu__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_qx6LXcI5pACvOu__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_qx6LXcI5pACvOu__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_qx6LXcI5pACvOu__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_qx6LXcI5pACvOu__, 0);

  lv_style_init(&GUI_Style__class_HxADxF6juDKWfB__);
  lv_style_set_bg_color(&GUI_Style__class_HxADxF6juDKWfB__,
                        lv_color_make(49, 49, 49));
  lv_style_set_bg_opa(&GUI_Style__class_HxADxF6juDKWfB__, 255);
  lv_style_set_radius(&GUI_Style__class_HxADxF6juDKWfB__, 5);
  lv_style_set_min_width(&GUI_Style__class_HxADxF6juDKWfB__, 0);
  lv_style_set_max_width(&GUI_Style__class_HxADxF6juDKWfB__, 8000);
  lv_style_set_min_height(&GUI_Style__class_HxADxF6juDKWfB__, 0);
  lv_style_set_max_height(&GUI_Style__class_HxADxF6juDKWfB__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_HxADxF6juDKWfB__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_HxADxF6juDKWfB__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_HxADxF6juDKWfB__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_HxADxF6juDKWfB__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_HxADxF6juDKWfB__, 0);

  lv_style_init(&GUI_Style__class_l2KF3gLqvrvgNA__);
  lv_style_set_pad_left(&GUI_Style__class_l2KF3gLqvrvgNA__, 16);
  lv_style_set_pad_right(&GUI_Style__class_l2KF3gLqvrvgNA__, 16);
  lv_style_set_pad_top(&GUI_Style__class_l2KF3gLqvrvgNA__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_l2KF3gLqvrvgNA__, 16);
  lv_style_set_flex_flow(&GUI_Style__class_l2KF3gLqvrvgNA__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_l2KF3gLqvrvgNA__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_l2KF3gLqvrvgNA__,
                                LV_FLEX_ALIGN_END);
  lv_style_set_flex_cross_place(&GUI_Style__class_l2KF3gLqvrvgNA__,
                                LV_FLEX_ALIGN_END);
  lv_style_set_pad_row(&GUI_Style__class_l2KF3gLqvrvgNA__, 0);
  lv_style_set_pad_column(&GUI_Style__class_l2KF3gLqvrvgNA__, 0);
  lv_style_set_layout(&GUI_Style__class_l2KF3gLqvrvgNA__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_QPHwtUHlUxUOEn__);
  lv_style_set_text_letter_space(&GUI_Style__class_QPHwtUHlUxUOEn__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_QPHwtUHlUxUOEn__, 0);
  lv_style_set_text_align(&GUI_Style__class_QPHwtUHlUxUOEn__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_QPHwtUHlUxUOEn__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_QPHwtUHlUxUOEn__, 255);
  lv_style_set_text_font(&GUI_Style__class_QPHwtUHlUxUOEn__, &number_1);

  lv_style_init(&GUI_Style__class_l05DXbHrWKpOYP__);
  lv_style_set_text_font(&GUI_Style__class_l05DXbHrWKpOYP__, &fixed23);
  lv_style_set_text_color(&GUI_Style__class_l05DXbHrWKpOYP__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_l05DXbHrWKpOYP__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_l05DXbHrWKpOYP__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_l05DXbHrWKpOYP__, 0);
  lv_style_set_text_align(&GUI_Style__class_l05DXbHrWKpOYP__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_lp2SW4VTZ90VZp__);
  lv_style_set_text_font(&GUI_Style__class_lp2SW4VTZ90VZp__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_lp2SW4VTZ90VZp__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_lp2SW4VTZ90VZp__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_lp2SW4VTZ90VZp__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_lp2SW4VTZ90VZp__, 0);
  lv_style_set_text_align(&GUI_Style__class_lp2SW4VTZ90VZp__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_S38iEdXYHp1zFH__);
  lv_style_set_text_font(&GUI_Style__class_S38iEdXYHp1zFH__, &display_1);
  lv_style_set_text_color(&GUI_Style__class_S38iEdXYHp1zFH__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_S38iEdXYHp1zFH__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_S38iEdXYHp1zFH__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_S38iEdXYHp1zFH__, 0);
  lv_style_set_text_align(&GUI_Style__class_S38iEdXYHp1zFH__,
                          LV_TEXT_ALIGN_CENTER);
  lv_style_set_pad_left(&GUI_Style__class_S38iEdXYHp1zFH__, 24);
  lv_style_set_pad_right(&GUI_Style__class_S38iEdXYHp1zFH__, 0);
  lv_style_set_pad_top(&GUI_Style__class_S38iEdXYHp1zFH__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_S38iEdXYHp1zFH__, 0);

  lv_style_init(&GUI_Style__class_kL3MCphSm1zdhB__);
  lv_style_set_pad_left(&GUI_Style__class_kL3MCphSm1zdhB__, 12);
  lv_style_set_pad_right(&GUI_Style__class_kL3MCphSm1zdhB__, 12);
  lv_style_set_pad_top(&GUI_Style__class_kL3MCphSm1zdhB__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_kL3MCphSm1zdhB__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_kL3MCphSm1zdhB__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_kL3MCphSm1zdhB__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_kL3MCphSm1zdhB__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_kL3MCphSm1zdhB__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_kL3MCphSm1zdhB__, 0);
  lv_style_set_pad_column(&GUI_Style__class_kL3MCphSm1zdhB__, 8);
  lv_style_set_layout(&GUI_Style__class_kL3MCphSm1zdhB__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_ouIAe8vEK7tLAC__);
  lv_style_set_min_width(&GUI_Style__class_ouIAe8vEK7tLAC__, 0);
  lv_style_set_max_width(&GUI_Style__class_ouIAe8vEK7tLAC__, 8000);
  lv_style_set_min_height(&GUI_Style__class_ouIAe8vEK7tLAC__, 0);
  lv_style_set_max_height(&GUI_Style__class_ouIAe8vEK7tLAC__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_ouIAe8vEK7tLAC__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_ouIAe8vEK7tLAC__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_ouIAe8vEK7tLAC__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_ouIAe8vEK7tLAC__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_ouIAe8vEK7tLAC__, 0);

  lv_style_init(&GUI_Style__class_OWp7o3fr8u7nZG__);
  lv_style_set_text_font(&GUI_Style__class_OWp7o3fr8u7nZG__, &fixed23);
  lv_style_set_text_color(&GUI_Style__class_OWp7o3fr8u7nZG__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_OWp7o3fr8u7nZG__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_OWp7o3fr8u7nZG__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_OWp7o3fr8u7nZG__, 0);
  lv_style_set_text_align(&GUI_Style__class_OWp7o3fr8u7nZG__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_dlCXVJg3k6UwKj__);
  lv_style_set_flex_flow(&GUI_Style__class_dlCXVJg3k6UwKj__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_dlCXVJg3k6UwKj__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_dlCXVJg3k6UwKj__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_dlCXVJg3k6UwKj__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_dlCXVJg3k6UwKj__, 0);
  lv_style_set_pad_column(&GUI_Style__class_dlCXVJg3k6UwKj__, 0);
  lv_style_set_layout(&GUI_Style__class_dlCXVJg3k6UwKj__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_UthKrP17uaRpGr__);
  lv_style_set_flex_flow(&GUI_Style__class_UthKrP17uaRpGr__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_UthKrP17uaRpGr__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_UthKrP17uaRpGr__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_UthKrP17uaRpGr__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_UthKrP17uaRpGr__, 6);
  lv_style_set_pad_column(&GUI_Style__class_UthKrP17uaRpGr__, 0);
  lv_style_set_layout(&GUI_Style__class_UthKrP17uaRpGr__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_7TxYpcbJKqrqzb__);
  lv_style_set_text_font(&GUI_Style__class_7TxYpcbJKqrqzb__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_7TxYpcbJKqrqzb__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_7TxYpcbJKqrqzb__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_7TxYpcbJKqrqzb__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_7TxYpcbJKqrqzb__, 0);
  lv_style_set_text_align(&GUI_Style__class_7TxYpcbJKqrqzb__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_fMZLS649q9lJQi__);
  lv_style_set_text_font(&GUI_Style__class_fMZLS649q9lJQi__, &header_1);
  lv_style_set_text_color(&GUI_Style__class_fMZLS649q9lJQi__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_fMZLS649q9lJQi__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_fMZLS649q9lJQi__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_fMZLS649q9lJQi__, 0);
  lv_style_set_text_align(&GUI_Style__class_fMZLS649q9lJQi__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_Jw5auITeCyGC1B__);
  lv_style_set_bg_color(&GUI_Style__class_Jw5auITeCyGC1B__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_Jw5auITeCyGC1B__, 255);
  lv_style_set_radius(&GUI_Style__class_Jw5auITeCyGC1B__, 0);
  lv_style_set_blend_mode(&GUI_Style__class_Jw5auITeCyGC1B__,
                          LV_BLEND_MODE_NORMAL);
  lv_style_set_opa(&GUI_Style__class_Jw5auITeCyGC1B__, 64);

  lv_style_init(&GUI_Style__class_3kssyjEdbcCLSh__);
  lv_style_set_flex_flow(&GUI_Style__class_3kssyjEdbcCLSh__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_3kssyjEdbcCLSh__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_3kssyjEdbcCLSh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_3kssyjEdbcCLSh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_3kssyjEdbcCLSh__, 6);
  lv_style_set_pad_column(&GUI_Style__class_3kssyjEdbcCLSh__, 0);
  lv_style_set_layout(&GUI_Style__class_3kssyjEdbcCLSh__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_JC5FT5PPZmgeAd__);
  lv_style_set_text_font(&GUI_Style__class_JC5FT5PPZmgeAd__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_JC5FT5PPZmgeAd__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_JC5FT5PPZmgeAd__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_JC5FT5PPZmgeAd__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_JC5FT5PPZmgeAd__, 0);
  lv_style_set_text_align(&GUI_Style__class_JC5FT5PPZmgeAd__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_z6I6jhPHmAziTD__);
  lv_style_set_text_font(&GUI_Style__class_z6I6jhPHmAziTD__, &header_1);
  lv_style_set_text_color(&GUI_Style__class_z6I6jhPHmAziTD__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_z6I6jhPHmAziTD__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_z6I6jhPHmAziTD__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_z6I6jhPHmAziTD__, 0);
  lv_style_set_text_align(&GUI_Style__class_z6I6jhPHmAziTD__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_u3dHPTQfScNGqf__);
  lv_style_set_bg_color(&GUI_Style__class_u3dHPTQfScNGqf__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_u3dHPTQfScNGqf__, 255);
  lv_style_set_radius(&GUI_Style__class_u3dHPTQfScNGqf__, 0);
  lv_style_set_blend_mode(&GUI_Style__class_u3dHPTQfScNGqf__,
                          LV_BLEND_MODE_NORMAL);
  lv_style_set_opa(&GUI_Style__class_u3dHPTQfScNGqf__, 64);

  lv_style_init(&GUI_Style__class_Kv9aIdJXPzd4E0__);
  lv_style_set_flex_flow(&GUI_Style__class_Kv9aIdJXPzd4E0__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_Kv9aIdJXPzd4E0__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_Kv9aIdJXPzd4E0__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_Kv9aIdJXPzd4E0__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_Kv9aIdJXPzd4E0__, 6);
  lv_style_set_pad_column(&GUI_Style__class_Kv9aIdJXPzd4E0__, 0);
  lv_style_set_layout(&GUI_Style__class_Kv9aIdJXPzd4E0__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_d6oNFVfxUQNLTp__);
  lv_style_set_text_font(&GUI_Style__class_d6oNFVfxUQNLTp__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_d6oNFVfxUQNLTp__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_d6oNFVfxUQNLTp__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_d6oNFVfxUQNLTp__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_d6oNFVfxUQNLTp__, 0);
  lv_style_set_text_align(&GUI_Style__class_d6oNFVfxUQNLTp__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_330bxbZF7kZsG8__);
  lv_style_set_flex_flow(&GUI_Style__class_330bxbZF7kZsG8__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_330bxbZF7kZsG8__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_330bxbZF7kZsG8__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_330bxbZF7kZsG8__,
                                LV_FLEX_ALIGN_END);
  lv_style_set_pad_row(&GUI_Style__class_330bxbZF7kZsG8__, 0);
  lv_style_set_pad_column(&GUI_Style__class_330bxbZF7kZsG8__, 2);
  lv_style_set_layout(&GUI_Style__class_330bxbZF7kZsG8__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_aB5O6v8av05RG3__);
  lv_style_set_text_font(&GUI_Style__class_aB5O6v8av05RG3__, &header_1);
  lv_style_set_text_color(&GUI_Style__class_aB5O6v8av05RG3__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_aB5O6v8av05RG3__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_aB5O6v8av05RG3__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_aB5O6v8av05RG3__, 0);
  lv_style_set_text_align(&GUI_Style__class_aB5O6v8av05RG3__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_E4wegkc5VJFKsz__);
  lv_style_set_pad_left(&GUI_Style__class_E4wegkc5VJFKsz__, 12);
  lv_style_set_pad_right(&GUI_Style__class_E4wegkc5VJFKsz__, 12);
  lv_style_set_pad_top(&GUI_Style__class_E4wegkc5VJFKsz__, 6);
  lv_style_set_pad_bottom(&GUI_Style__class_E4wegkc5VJFKsz__, 10);
  lv_style_set_flex_flow(&GUI_Style__class_E4wegkc5VJFKsz__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_E4wegkc5VJFKsz__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_E4wegkc5VJFKsz__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_E4wegkc5VJFKsz__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_E4wegkc5VJFKsz__, 0);
  lv_style_set_pad_column(&GUI_Style__class_E4wegkc5VJFKsz__, 0);
  lv_style_set_layout(&GUI_Style__class_E4wegkc5VJFKsz__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_txXxHpeZgsni8H__);
  lv_style_set_pad_left(&GUI_Style__class_txXxHpeZgsni8H__, 0);
  lv_style_set_pad_right(&GUI_Style__class_txXxHpeZgsni8H__, 0);
  lv_style_set_pad_top(&GUI_Style__class_txXxHpeZgsni8H__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_txXxHpeZgsni8H__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_txXxHpeZgsni8H__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_txXxHpeZgsni8H__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_txXxHpeZgsni8H__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_txXxHpeZgsni8H__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_txXxHpeZgsni8H__, 0);
  lv_style_set_pad_column(&GUI_Style__class_txXxHpeZgsni8H__, 0);
  lv_style_set_layout(&GUI_Style__class_txXxHpeZgsni8H__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_ePErXDcnIvioYq__);
  lv_style_set_text_letter_space(&GUI_Style__class_ePErXDcnIvioYq__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_ePErXDcnIvioYq__, 0);
  lv_style_set_text_align(&GUI_Style__class_ePErXDcnIvioYq__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_ePErXDcnIvioYq__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_ePErXDcnIvioYq__, 255);
  lv_style_set_text_font(&GUI_Style__class_ePErXDcnIvioYq__, &label_1);
  lv_style_set_pad_left(&GUI_Style__class_ePErXDcnIvioYq__, 0);
  lv_style_set_pad_right(&GUI_Style__class_ePErXDcnIvioYq__, 5);
  lv_style_set_pad_top(&GUI_Style__class_ePErXDcnIvioYq__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_ePErXDcnIvioYq__, 0);

  lv_style_init(&GUI_Style__class_orAle4UexZ0qwV__);
  lv_style_set_text_letter_space(&GUI_Style__class_orAle4UexZ0qwV__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_orAle4UexZ0qwV__, 0);
  lv_style_set_text_align(&GUI_Style__class_orAle4UexZ0qwV__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_orAle4UexZ0qwV__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_orAle4UexZ0qwV__, 255);
  lv_style_set_text_font(&GUI_Style__class_orAle4UexZ0qwV__, &label_1);

  lv_style_init(&GUI_Style__class_NHMYIepongiLxJ__);
  lv_style_set_pad_left(&GUI_Style__class_NHMYIepongiLxJ__, 0);
  lv_style_set_pad_right(&GUI_Style__class_NHMYIepongiLxJ__, 0);
  lv_style_set_pad_top(&GUI_Style__class_NHMYIepongiLxJ__, 0);
  lv_style_set_pad_bottom(&GUI_Style__class_NHMYIepongiLxJ__, 7);
  lv_style_set_flex_flow(&GUI_Style__class_NHMYIepongiLxJ__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_NHMYIepongiLxJ__,
                               LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_style_set_flex_track_place(&GUI_Style__class_NHMYIepongiLxJ__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_NHMYIepongiLxJ__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_NHMYIepongiLxJ__, 0);
  lv_style_set_pad_column(&GUI_Style__class_NHMYIepongiLxJ__, 0);
  lv_style_set_layout(&GUI_Style__class_NHMYIepongiLxJ__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_bnXaTUcRtszjic__);
  lv_style_set_flex_flow(&GUI_Style__class_bnXaTUcRtszjic__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_bnXaTUcRtszjic__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_bnXaTUcRtszjic__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_bnXaTUcRtszjic__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_bnXaTUcRtszjic__, 7);
  lv_style_set_pad_column(&GUI_Style__class_bnXaTUcRtszjic__, 0);
  lv_style_set_layout(&GUI_Style__class_bnXaTUcRtszjic__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_e1RJqKV5YEtyLL__);
  lv_style_set_text_font(&GUI_Style__class_e1RJqKV5YEtyLL__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_e1RJqKV5YEtyLL__,
                          lv_color_make(255, 145, 0));
  lv_style_set_text_opa(&GUI_Style__class_e1RJqKV5YEtyLL__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_e1RJqKV5YEtyLL__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_e1RJqKV5YEtyLL__, 0);
  lv_style_set_text_align(&GUI_Style__class_e1RJqKV5YEtyLL__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_UwhUwo9ljeO8lt__);
  lv_style_set_opa(&GUI_Style__class_UwhUwo9ljeO8lt__, 255);

  lv_style_init(&GUI_Style__class_z1zvszPSytj7GK__);
  lv_style_set_text_font(&GUI_Style__class_z1zvszPSytj7GK__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_z1zvszPSytj7GK__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_z1zvszPSytj7GK__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_z1zvszPSytj7GK__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_z1zvszPSytj7GK__, 0);
  lv_style_set_text_align(&GUI_Style__class_z1zvszPSytj7GK__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_SqnJ2yTfN70m5F__);
  lv_style_set_flex_flow(&GUI_Style__class_SqnJ2yTfN70m5F__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_SqnJ2yTfN70m5F__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_SqnJ2yTfN70m5F__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_SqnJ2yTfN70m5F__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_SqnJ2yTfN70m5F__, 7);
  lv_style_set_pad_column(&GUI_Style__class_SqnJ2yTfN70m5F__, 0);
  lv_style_set_layout(&GUI_Style__class_SqnJ2yTfN70m5F__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_Sl2id12bl9NQMv__);
  lv_style_set_text_font(&GUI_Style__class_Sl2id12bl9NQMv__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_Sl2id12bl9NQMv__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_Sl2id12bl9NQMv__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_Sl2id12bl9NQMv__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_Sl2id12bl9NQMv__, 0);
  lv_style_set_text_align(&GUI_Style__class_Sl2id12bl9NQMv__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_Yoie1JkY7ut7dR__);
  lv_style_set_text_font(&GUI_Style__class_Yoie1JkY7ut7dR__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_Yoie1JkY7ut7dR__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_Yoie1JkY7ut7dR__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_Yoie1JkY7ut7dR__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_Yoie1JkY7ut7dR__, 0);
  lv_style_set_text_align(&GUI_Style__class_Yoie1JkY7ut7dR__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_9V6WvrW17HbJD1__);
  lv_style_set_flex_flow(&GUI_Style__class_9V6WvrW17HbJD1__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_9V6WvrW17HbJD1__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_9V6WvrW17HbJD1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_9V6WvrW17HbJD1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_9V6WvrW17HbJD1__, 7);
  lv_style_set_pad_column(&GUI_Style__class_9V6WvrW17HbJD1__, 0);
  lv_style_set_layout(&GUI_Style__class_9V6WvrW17HbJD1__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_uctqe13XSbRVrf__);
  lv_style_set_text_font(&GUI_Style__class_uctqe13XSbRVrf__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_uctqe13XSbRVrf__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_uctqe13XSbRVrf__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_uctqe13XSbRVrf__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_uctqe13XSbRVrf__, 0);
  lv_style_set_text_align(&GUI_Style__class_uctqe13XSbRVrf__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_CrXq0J1lzKaWzo__);
  lv_style_set_text_font(&GUI_Style__class_CrXq0J1lzKaWzo__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_CrXq0J1lzKaWzo__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_CrXq0J1lzKaWzo__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_CrXq0J1lzKaWzo__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_CrXq0J1lzKaWzo__, 0);
  lv_style_set_text_align(&GUI_Style__class_CrXq0J1lzKaWzo__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_yL6megXhm9GYb7__);
  lv_style_set_flex_flow(&GUI_Style__class_yL6megXhm9GYb7__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_yL6megXhm9GYb7__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_yL6megXhm9GYb7__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_yL6megXhm9GYb7__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_yL6megXhm9GYb7__, 7);
  lv_style_set_pad_column(&GUI_Style__class_yL6megXhm9GYb7__, 0);
  lv_style_set_layout(&GUI_Style__class_yL6megXhm9GYb7__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_4FP9fwNC3Jfkoc__);
  lv_style_set_text_font(&GUI_Style__class_4FP9fwNC3Jfkoc__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_4FP9fwNC3Jfkoc__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_4FP9fwNC3Jfkoc__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_4FP9fwNC3Jfkoc__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_4FP9fwNC3Jfkoc__, 0);
  lv_style_set_text_align(&GUI_Style__class_4FP9fwNC3Jfkoc__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_wWU5ITomt2XhrC__);
  lv_style_set_text_font(&GUI_Style__class_wWU5ITomt2XhrC__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_wWU5ITomt2XhrC__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_wWU5ITomt2XhrC__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_wWU5ITomt2XhrC__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_wWU5ITomt2XhrC__, 0);
  lv_style_set_text_align(&GUI_Style__class_wWU5ITomt2XhrC__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_u9F6wP0qFMftID__);
  lv_style_set_flex_flow(&GUI_Style__class_u9F6wP0qFMftID__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_u9F6wP0qFMftID__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_u9F6wP0qFMftID__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_u9F6wP0qFMftID__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_u9F6wP0qFMftID__, 7);
  lv_style_set_pad_column(&GUI_Style__class_u9F6wP0qFMftID__, 0);
  lv_style_set_layout(&GUI_Style__class_u9F6wP0qFMftID__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_zkJojhqYN6amU5__);
  lv_style_set_text_font(&GUI_Style__class_zkJojhqYN6amU5__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_zkJojhqYN6amU5__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_zkJojhqYN6amU5__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_zkJojhqYN6amU5__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_zkJojhqYN6amU5__, 0);
  lv_style_set_text_align(&GUI_Style__class_zkJojhqYN6amU5__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_zMg69aamtHeGHF__);
  lv_style_set_text_font(&GUI_Style__class_zMg69aamtHeGHF__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_zMg69aamtHeGHF__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_zMg69aamtHeGHF__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_zMg69aamtHeGHF__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_zMg69aamtHeGHF__, 0);
  lv_style_set_text_align(&GUI_Style__class_zMg69aamtHeGHF__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_N6QjKxi1tUPVcv__);
  lv_style_set_pad_left(&GUI_Style__class_N6QjKxi1tUPVcv__, 12);
  lv_style_set_pad_right(&GUI_Style__class_N6QjKxi1tUPVcv__, 12);
  lv_style_set_pad_top(&GUI_Style__class_N6QjKxi1tUPVcv__, 6);
  lv_style_set_pad_bottom(&GUI_Style__class_N6QjKxi1tUPVcv__, 12);
  lv_style_set_flex_flow(&GUI_Style__class_N6QjKxi1tUPVcv__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_N6QjKxi1tUPVcv__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_N6QjKxi1tUPVcv__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_N6QjKxi1tUPVcv__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_N6QjKxi1tUPVcv__, 0);
  lv_style_set_pad_column(&GUI_Style__class_N6QjKxi1tUPVcv__, 0);
  lv_style_set_layout(&GUI_Style__class_N6QjKxi1tUPVcv__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_WhQHp8mtjRM1Z6__);
  lv_style_set_flex_flow(&GUI_Style__class_WhQHp8mtjRM1Z6__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_WhQHp8mtjRM1Z6__,
                               LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_style_set_flex_track_place(&GUI_Style__class_WhQHp8mtjRM1Z6__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_WhQHp8mtjRM1Z6__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_WhQHp8mtjRM1Z6__, 0);
  lv_style_set_pad_column(&GUI_Style__class_WhQHp8mtjRM1Z6__, 1);
  lv_style_set_layout(&GUI_Style__class_WhQHp8mtjRM1Z6__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_6Zqyl1PAzGby72__);
  lv_style_set_flex_flow(&GUI_Style__class_6Zqyl1PAzGby72__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_6Zqyl1PAzGby72__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_6Zqyl1PAzGby72__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_6Zqyl1PAzGby72__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_6Zqyl1PAzGby72__, 4);
  lv_style_set_pad_column(&GUI_Style__class_6Zqyl1PAzGby72__, 0);
  lv_style_set_layout(&GUI_Style__class_6Zqyl1PAzGby72__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_P20EIfA3luXdY3__);
  lv_style_set_bg_color(&GUI_Style__class_P20EIfA3luXdY3__,
                        lv_color_make(67, 66, 64));
  lv_style_set_bg_opa(&GUI_Style__class_P20EIfA3luXdY3__, 0);
  lv_style_set_radius(&GUI_Style__class_P20EIfA3luXdY3__, 16);
  lv_style_set_flex_flow(&GUI_Style__class_P20EIfA3luXdY3__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_P20EIfA3luXdY3__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_P20EIfA3luXdY3__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_P20EIfA3luXdY3__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_P20EIfA3luXdY3__, 4);
  lv_style_set_pad_column(&GUI_Style__class_P20EIfA3luXdY3__, 0);
  lv_style_set_layout(&GUI_Style__class_P20EIfA3luXdY3__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_mGTxkagT6y6pNF__);
  lv_style_set_text_font(&GUI_Style__class_mGTxkagT6y6pNF__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_mGTxkagT6y6pNF__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_mGTxkagT6y6pNF__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_mGTxkagT6y6pNF__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_mGTxkagT6y6pNF__, 0);
  lv_style_set_text_align(&GUI_Style__class_mGTxkagT6y6pNF__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_QtBH9YSEMkGaW1__);
  lv_style_set_text_font(&GUI_Style__class_QtBH9YSEMkGaW1__, &title_1);
  lv_style_set_text_color(&GUI_Style__class_QtBH9YSEMkGaW1__,
                          lv_color_make(255, 145, 0));
  lv_style_set_text_opa(&GUI_Style__class_QtBH9YSEMkGaW1__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_QtBH9YSEMkGaW1__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_QtBH9YSEMkGaW1__, 0);
  lv_style_set_text_align(&GUI_Style__class_QtBH9YSEMkGaW1__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_9MpPQcvJSEks3y__);
  lv_style_set_text_letter_space(&GUI_Style__class_9MpPQcvJSEks3y__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_9MpPQcvJSEks3y__, 0);
  lv_style_set_text_align(&GUI_Style__class_9MpPQcvJSEks3y__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_9MpPQcvJSEks3y__,
                          lv_color_make(140, 187, 252));
  lv_style_set_text_opa(&GUI_Style__class_9MpPQcvJSEks3y__, 255);
  lv_style_set_text_font(&GUI_Style__class_9MpPQcvJSEks3y__, &title_1);

  lv_style_init(&GUI_Style__class_wlkSJFhc3canUg__);
  lv_style_set_text_font(&GUI_Style__class_wlkSJFhc3canUg__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_wlkSJFhc3canUg__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_wlkSJFhc3canUg__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_wlkSJFhc3canUg__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_wlkSJFhc3canUg__, 0);
  lv_style_set_text_align(&GUI_Style__class_wlkSJFhc3canUg__,
                          LV_TEXT_ALIGN_CENTER);

  lv_style_init(&GUI_Style__class_OvMbXZAsPbZjnb__);
  lv_style_set_bg_color(&GUI_Style__class_OvMbXZAsPbZjnb__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_OvMbXZAsPbZjnb__, 255);
  lv_style_set_radius(&GUI_Style__class_OvMbXZAsPbZjnb__, 0);
  lv_style_set_blend_mode(&GUI_Style__class_OvMbXZAsPbZjnb__,
                          LV_BLEND_MODE_NORMAL);
  lv_style_set_opa(&GUI_Style__class_OvMbXZAsPbZjnb__, 64);

  lv_style_init(&GUI_Style__class_E4t2gqcHR8MbAP__);
  lv_style_set_flex_flow(&GUI_Style__class_E4t2gqcHR8MbAP__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_E4t2gqcHR8MbAP__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_E4t2gqcHR8MbAP__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_E4t2gqcHR8MbAP__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_E4t2gqcHR8MbAP__, 4);
  lv_style_set_pad_column(&GUI_Style__class_E4t2gqcHR8MbAP__, 0);
  lv_style_set_layout(&GUI_Style__class_E4t2gqcHR8MbAP__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_XwvnqhCeV2aoM1__);
  lv_style_set_bg_color(&GUI_Style__class_XwvnqhCeV2aoM1__,
                        lv_color_make(67, 66, 64));
  lv_style_set_bg_opa(&GUI_Style__class_XwvnqhCeV2aoM1__, 0);
  lv_style_set_radius(&GUI_Style__class_XwvnqhCeV2aoM1__, 16);
  lv_style_set_flex_flow(&GUI_Style__class_XwvnqhCeV2aoM1__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_XwvnqhCeV2aoM1__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_XwvnqhCeV2aoM1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_XwvnqhCeV2aoM1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_XwvnqhCeV2aoM1__, 4);
  lv_style_set_pad_column(&GUI_Style__class_XwvnqhCeV2aoM1__, 0);
  lv_style_set_layout(&GUI_Style__class_XwvnqhCeV2aoM1__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_1w85YzMTP9d5q4__);
  lv_style_set_text_font(&GUI_Style__class_1w85YzMTP9d5q4__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_1w85YzMTP9d5q4__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_1w85YzMTP9d5q4__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_1w85YzMTP9d5q4__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_1w85YzMTP9d5q4__, 0);
  lv_style_set_text_align(&GUI_Style__class_1w85YzMTP9d5q4__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_j3PIJOujL57xFq__);
  lv_style_set_text_font(&GUI_Style__class_j3PIJOujL57xFq__, &title_1);
  lv_style_set_text_color(&GUI_Style__class_j3PIJOujL57xFq__,
                          lv_color_make(255, 145, 0));
  lv_style_set_text_opa(&GUI_Style__class_j3PIJOujL57xFq__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_j3PIJOujL57xFq__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_j3PIJOujL57xFq__, 0);
  lv_style_set_text_align(&GUI_Style__class_j3PIJOujL57xFq__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_wKOu7L7fJpWUc8__);
  lv_style_set_text_letter_space(&GUI_Style__class_wKOu7L7fJpWUc8__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_wKOu7L7fJpWUc8__, 0);
  lv_style_set_text_align(&GUI_Style__class_wKOu7L7fJpWUc8__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_wKOu7L7fJpWUc8__,
                          lv_color_make(140, 187, 252));
  lv_style_set_text_opa(&GUI_Style__class_wKOu7L7fJpWUc8__, 255);
  lv_style_set_text_font(&GUI_Style__class_wKOu7L7fJpWUc8__, &title_1);

  lv_style_init(&GUI_Style__class_DaDvAetiVI0Sy4__);
  lv_style_set_text_font(&GUI_Style__class_DaDvAetiVI0Sy4__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_DaDvAetiVI0Sy4__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_DaDvAetiVI0Sy4__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_DaDvAetiVI0Sy4__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_DaDvAetiVI0Sy4__, 0);
  lv_style_set_text_align(&GUI_Style__class_DaDvAetiVI0Sy4__,
                          LV_TEXT_ALIGN_CENTER);

  lv_style_init(&GUI_Style__class_HZ1hQzhR4glwPE__);
  lv_style_set_bg_color(&GUI_Style__class_HZ1hQzhR4glwPE__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_HZ1hQzhR4glwPE__, 255);
  lv_style_set_radius(&GUI_Style__class_HZ1hQzhR4glwPE__, 0);
  lv_style_set_blend_mode(&GUI_Style__class_HZ1hQzhR4glwPE__,
                          LV_BLEND_MODE_NORMAL);
  lv_style_set_opa(&GUI_Style__class_HZ1hQzhR4glwPE__, 64);

  lv_style_init(&GUI_Style__class_sI3TdUhM0WS1Zb__);
  lv_style_set_flex_flow(&GUI_Style__class_sI3TdUhM0WS1Zb__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_sI3TdUhM0WS1Zb__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_sI3TdUhM0WS1Zb__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_sI3TdUhM0WS1Zb__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_sI3TdUhM0WS1Zb__, 4);
  lv_style_set_pad_column(&GUI_Style__class_sI3TdUhM0WS1Zb__, 0);
  lv_style_set_layout(&GUI_Style__class_sI3TdUhM0WS1Zb__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_f9uFiY7jL0YFTZ__);
  lv_style_set_bg_color(&GUI_Style__class_f9uFiY7jL0YFTZ__,
                        lv_color_make(67, 66, 64));
  lv_style_set_bg_opa(&GUI_Style__class_f9uFiY7jL0YFTZ__, 0);
  lv_style_set_radius(&GUI_Style__class_f9uFiY7jL0YFTZ__, 16);
  lv_style_set_flex_flow(&GUI_Style__class_f9uFiY7jL0YFTZ__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_f9uFiY7jL0YFTZ__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_f9uFiY7jL0YFTZ__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_f9uFiY7jL0YFTZ__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_f9uFiY7jL0YFTZ__, 4);
  lv_style_set_pad_column(&GUI_Style__class_f9uFiY7jL0YFTZ__, 0);
  lv_style_set_layout(&GUI_Style__class_f9uFiY7jL0YFTZ__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_tpmJOiEkbfWCQ5__);
  lv_style_set_text_font(&GUI_Style__class_tpmJOiEkbfWCQ5__, &subtitle_1);
  lv_style_set_text_color(&GUI_Style__class_tpmJOiEkbfWCQ5__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_tpmJOiEkbfWCQ5__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_tpmJOiEkbfWCQ5__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_tpmJOiEkbfWCQ5__, 0);
  lv_style_set_text_align(&GUI_Style__class_tpmJOiEkbfWCQ5__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_NqL7LHebj5jnE1__);
  lv_style_set_text_font(&GUI_Style__class_NqL7LHebj5jnE1__, &title_1);
  lv_style_set_text_color(&GUI_Style__class_NqL7LHebj5jnE1__,
                          lv_color_make(255, 145, 0));
  lv_style_set_text_opa(&GUI_Style__class_NqL7LHebj5jnE1__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_NqL7LHebj5jnE1__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_NqL7LHebj5jnE1__, 0);
  lv_style_set_text_align(&GUI_Style__class_NqL7LHebj5jnE1__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_5aC5gkRCTJRBpX__);
  lv_style_set_text_letter_space(&GUI_Style__class_5aC5gkRCTJRBpX__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_5aC5gkRCTJRBpX__, 0);
  lv_style_set_text_align(&GUI_Style__class_5aC5gkRCTJRBpX__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_5aC5gkRCTJRBpX__,
                          lv_color_make(140, 187, 252));
  lv_style_set_text_opa(&GUI_Style__class_5aC5gkRCTJRBpX__, 255);
  lv_style_set_text_font(&GUI_Style__class_5aC5gkRCTJRBpX__, &title_1);

  lv_style_init(&GUI_Style__class_MNThGQl2quXcmK__);
  lv_style_set_text_font(&GUI_Style__class_MNThGQl2quXcmK__, &label_1);
  lv_style_set_text_color(&GUI_Style__class_MNThGQl2quXcmK__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_MNThGQl2quXcmK__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_MNThGQl2quXcmK__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_MNThGQl2quXcmK__, 0);
  lv_style_set_text_align(&GUI_Style__class_MNThGQl2quXcmK__,
                          LV_TEXT_ALIGN_CENTER);

  lv_style_init(&GUI_Style__class_292pAkrGlFbI2Z__);
  lv_style_set_min_width(&GUI_Style__class_292pAkrGlFbI2Z__, 0);
  lv_style_set_max_width(&GUI_Style__class_292pAkrGlFbI2Z__, 8000);
  lv_style_set_min_height(&GUI_Style__class_292pAkrGlFbI2Z__, 0);
  lv_style_set_max_height(&GUI_Style__class_292pAkrGlFbI2Z__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_292pAkrGlFbI2Z__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_292pAkrGlFbI2Z__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_292pAkrGlFbI2Z__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_292pAkrGlFbI2Z__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_292pAkrGlFbI2Z__, 0);

  lv_style_init(&GUI_Style__class_5TGhYvIVJqeKbz__);
  lv_style_set_bg_color(&GUI_Style__class_5TGhYvIVJqeKbz__,
                        lv_color_make(49, 49, 49));
  lv_style_set_bg_opa(&GUI_Style__class_5TGhYvIVJqeKbz__, 255);
  lv_style_set_radius(&GUI_Style__class_5TGhYvIVJqeKbz__, 5);

  lv_style_init(&GUI_Style__class_q2R0whE4QSYoQW__);
  lv_style_set_flex_flow(&GUI_Style__class_q2R0whE4QSYoQW__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_q2R0whE4QSYoQW__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_q2R0whE4QSYoQW__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_q2R0whE4QSYoQW__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_q2R0whE4QSYoQW__, 0);
  lv_style_set_pad_column(&GUI_Style__class_q2R0whE4QSYoQW__, 0);
  lv_style_set_layout(&GUI_Style__class_q2R0whE4QSYoQW__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_AvvcYB4wlpzNqr__);
  lv_style_set_bg_color(&GUI_Style__class_AvvcYB4wlpzNqr__,
                        lv_color_make(95, 95, 95));
  lv_style_set_bg_opa(&GUI_Style__class_AvvcYB4wlpzNqr__, 255);
  lv_style_set_radius(&GUI_Style__class_AvvcYB4wlpzNqr__, 0);

  lv_style_init(&GUI_Style__class_iwIHCP82XdjJdR__);
  lv_style_set_min_width(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_max_width(&GUI_Style__class_iwIHCP82XdjJdR__, 8000);
  lv_style_set_min_height(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_max_height(&GUI_Style__class_iwIHCP82XdjJdR__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_iwIHCP82XdjJdR__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_iwIHCP82XdjJdR__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_iwIHCP82XdjJdR__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_iwIHCP82XdjJdR__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_iwIHCP82XdjJdR__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_iwIHCP82XdjJdR__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_iwIHCP82XdjJdR__, 10);
  lv_style_set_pad_column(&GUI_Style__class_iwIHCP82XdjJdR__, 0);
  lv_style_set_layout(&GUI_Style__class_iwIHCP82XdjJdR__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_3D1LJzVb3VpFXw__);
  lv_style_set_text_font(&GUI_Style__class_3D1LJzVb3VpFXw__, &number_1);
  lv_style_set_text_color(&GUI_Style__class_3D1LJzVb3VpFXw__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_3D1LJzVb3VpFXw__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_text_align(&GUI_Style__class_3D1LJzVb3VpFXw__,
                          LV_TEXT_ALIGN_CENTER);
  lv_style_set_bg_color(&GUI_Style__class_3D1LJzVb3VpFXw__,
                        lv_color_make(0, 0, 0));
  lv_style_set_bg_opa(&GUI_Style__class_3D1LJzVb3VpFXw__, 255);
  lv_style_set_radius(&GUI_Style__class_3D1LJzVb3VpFXw__, 5);
  lv_style_set_outline_color(&GUI_Style__class_3D1LJzVb3VpFXw__,
                             lv_color_make(155, 155, 155));
  lv_style_set_outline_opa(&GUI_Style__class_3D1LJzVb3VpFXw__, 255);
  lv_style_set_outline_width(&GUI_Style__class_3D1LJzVb3VpFXw__, 3);
  lv_style_set_outline_pad(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_min_width(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_max_width(&GUI_Style__class_3D1LJzVb3VpFXw__, 8000);
  lv_style_set_min_height(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_max_height(&GUI_Style__class_3D1LJzVb3VpFXw__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_3D1LJzVb3VpFXw__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_3D1LJzVb3VpFXw__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_3D1LJzVb3VpFXw__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_3D1LJzVb3VpFXw__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_3D1LJzVb3VpFXw__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_3D1LJzVb3VpFXw__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_pad_column(&GUI_Style__class_3D1LJzVb3VpFXw__, 0);
  lv_style_set_layout(&GUI_Style__class_3D1LJzVb3VpFXw__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_T55yx89GcErjMB__);
  lv_style_set_outline_color(&GUI_Style__class_T55yx89GcErjMB__,
                             lv_color_make(155, 155, 155));
  lv_style_set_outline_opa(&GUI_Style__class_T55yx89GcErjMB__, 255);
  lv_style_set_outline_width(&GUI_Style__class_T55yx89GcErjMB__, 3);
  lv_style_set_outline_pad(&GUI_Style__class_T55yx89GcErjMB__, 0);

  lv_style_init(&GUI_Style__class_N2ZGjd4KyT2jge__);
  lv_style_set_outline_color(&GUI_Style__class_N2ZGjd4KyT2jge__,
                             lv_color_make(0, 0, 0));
  lv_style_set_outline_opa(&GUI_Style__class_N2ZGjd4KyT2jge__, 255);
  lv_style_set_outline_width(&GUI_Style__class_N2ZGjd4KyT2jge__, 2);
  lv_style_set_outline_pad(&GUI_Style__class_N2ZGjd4KyT2jge__, 0);

  lv_style_init(&GUI_Style__class_rR3z1AvlAdsSFC__);
  lv_style_set_flex_flow(&GUI_Style__class_rR3z1AvlAdsSFC__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_rR3z1AvlAdsSFC__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_rR3z1AvlAdsSFC__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_rR3z1AvlAdsSFC__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_rR3z1AvlAdsSFC__, 0);
  lv_style_set_pad_column(&GUI_Style__class_rR3z1AvlAdsSFC__, 30);
  lv_style_set_layout(&GUI_Style__class_rR3z1AvlAdsSFC__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_woKeZ5erBDCvcw__);
  lv_style_set_bg_color(&GUI_Style__class_woKeZ5erBDCvcw__,
                        lv_color_make(0, 125, 48));
  lv_style_set_bg_opa(&GUI_Style__class_woKeZ5erBDCvcw__, 255);
  lv_style_set_radius(&GUI_Style__class_woKeZ5erBDCvcw__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_woKeZ5erBDCvcw__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_woKeZ5erBDCvcw__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_woKeZ5erBDCvcw__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_woKeZ5erBDCvcw__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_woKeZ5erBDCvcw__, 0);
  lv_style_set_pad_column(&GUI_Style__class_woKeZ5erBDCvcw__, 0);
  lv_style_set_layout(&GUI_Style__class_woKeZ5erBDCvcw__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_nBXuUR4hpWx00U__);
  lv_style_set_text_color(&GUI_Style__class_nBXuUR4hpWx00U__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_nBXuUR4hpWx00U__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_nBXuUR4hpWx00U__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_nBXuUR4hpWx00U__, 0);
  lv_style_set_text_align(&GUI_Style__class_nBXuUR4hpWx00U__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_RW9Sw1n3v6RTgh__);
  lv_style_set_bg_color(&GUI_Style__class_RW9Sw1n3v6RTgh__,
                        lv_color_make(180, 1, 1));
  lv_style_set_bg_opa(&GUI_Style__class_RW9Sw1n3v6RTgh__, 255);
  lv_style_set_radius(&GUI_Style__class_RW9Sw1n3v6RTgh__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_RW9Sw1n3v6RTgh__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_RW9Sw1n3v6RTgh__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_RW9Sw1n3v6RTgh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_RW9Sw1n3v6RTgh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_RW9Sw1n3v6RTgh__, 0);
  lv_style_set_pad_column(&GUI_Style__class_RW9Sw1n3v6RTgh__, 0);
  lv_style_set_layout(&GUI_Style__class_RW9Sw1n3v6RTgh__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_UgrmHRDfBkkD15__);
  lv_style_set_text_color(&GUI_Style__class_UgrmHRDfBkkD15__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_UgrmHRDfBkkD15__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_UgrmHRDfBkkD15__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_UgrmHRDfBkkD15__, 0);
  lv_style_set_text_align(&GUI_Style__class_UgrmHRDfBkkD15__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_dVaeR4MlNk6JVP__);
  lv_style_set_bg_color(&GUI_Style__class_dVaeR4MlNk6JVP__,
                        lv_color_make(95, 95, 95));
  lv_style_set_bg_opa(&GUI_Style__class_dVaeR4MlNk6JVP__, 255);
  lv_style_set_radius(&GUI_Style__class_dVaeR4MlNk6JVP__, 0);

  lv_style_init(&GUI_Style__class_vksvC08E9lfusD__);
  lv_style_set_min_width(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_max_width(&GUI_Style__class_vksvC08E9lfusD__, 8000);
  lv_style_set_min_height(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_max_height(&GUI_Style__class_vksvC08E9lfusD__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_vksvC08E9lfusD__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_vksvC08E9lfusD__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_vksvC08E9lfusD__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_vksvC08E9lfusD__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_vksvC08E9lfusD__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_vksvC08E9lfusD__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_vksvC08E9lfusD__, 10);
  lv_style_set_pad_column(&GUI_Style__class_vksvC08E9lfusD__, 0);
  lv_style_set_layout(&GUI_Style__class_vksvC08E9lfusD__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_fqJ9BAuupzIIWh__);
  lv_style_set_bg_color(&GUI_Style__class_fqJ9BAuupzIIWh__,
                        lv_color_make(0, 0, 0));
  lv_style_set_bg_opa(&GUI_Style__class_fqJ9BAuupzIIWh__, 255);
  lv_style_set_radius(&GUI_Style__class_fqJ9BAuupzIIWh__, 5);
  lv_style_set_outline_color(&GUI_Style__class_fqJ9BAuupzIIWh__,
                             lv_color_make(155, 155, 155));
  lv_style_set_outline_opa(&GUI_Style__class_fqJ9BAuupzIIWh__, 255);
  lv_style_set_outline_pad(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_outline_width(&GUI_Style__class_fqJ9BAuupzIIWh__, 3);
  lv_style_set_max_height(&GUI_Style__class_fqJ9BAuupzIIWh__, 8000);
  lv_style_set_max_width(&GUI_Style__class_fqJ9BAuupzIIWh__, 8000);
  lv_style_set_min_height(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_min_width(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_transform_rotation(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_transform_scale_x(&GUI_Style__class_fqJ9BAuupzIIWh__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_fqJ9BAuupzIIWh__, 256);
  lv_style_set_text_letter_space(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_text_align(&GUI_Style__class_fqJ9BAuupzIIWh__,
                          LV_TEXT_ALIGN_CENTER);
  lv_style_set_text_color(&GUI_Style__class_fqJ9BAuupzIIWh__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_fqJ9BAuupzIIWh__, 255);
  lv_style_set_text_font(&GUI_Style__class_fqJ9BAuupzIIWh__, &number_1);
  lv_style_set_flex_flow(&GUI_Style__class_fqJ9BAuupzIIWh__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_fqJ9BAuupzIIWh__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_fqJ9BAuupzIIWh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_fqJ9BAuupzIIWh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_pad_column(&GUI_Style__class_fqJ9BAuupzIIWh__, 0);
  lv_style_set_layout(&GUI_Style__class_fqJ9BAuupzIIWh__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_3VuxfhxbEdo6EO__);
  lv_style_set_outline_color(&GUI_Style__class_3VuxfhxbEdo6EO__,
                             lv_color_make(155, 155, 155));
  lv_style_set_outline_opa(&GUI_Style__class_3VuxfhxbEdo6EO__, 255);
  lv_style_set_outline_pad(&GUI_Style__class_3VuxfhxbEdo6EO__, 0);
  lv_style_set_outline_width(&GUI_Style__class_3VuxfhxbEdo6EO__, 3);

  lv_style_init(&GUI_Style__class_EQHHuxrbsUsFwQ__);
  lv_style_set_outline_color(&GUI_Style__class_EQHHuxrbsUsFwQ__,
                             lv_color_make(0, 0, 0));
  lv_style_set_outline_opa(&GUI_Style__class_EQHHuxrbsUsFwQ__, 255);
  lv_style_set_outline_width(&GUI_Style__class_EQHHuxrbsUsFwQ__, 2);
  lv_style_set_outline_pad(&GUI_Style__class_EQHHuxrbsUsFwQ__, 0);

  lv_style_init(&GUI_Style__class_YQdvrXqq6m6UwT__);
  lv_style_set_flex_flow(&GUI_Style__class_YQdvrXqq6m6UwT__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_YQdvrXqq6m6UwT__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_YQdvrXqq6m6UwT__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_YQdvrXqq6m6UwT__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_YQdvrXqq6m6UwT__, 0);
  lv_style_set_pad_column(&GUI_Style__class_YQdvrXqq6m6UwT__, 30);
  lv_style_set_layout(&GUI_Style__class_YQdvrXqq6m6UwT__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_5dd3lX6oAKILzH__);
  lv_style_set_bg_color(&GUI_Style__class_5dd3lX6oAKILzH__,
                        lv_color_make(0, 125, 48));
  lv_style_set_bg_opa(&GUI_Style__class_5dd3lX6oAKILzH__, 255);
  lv_style_set_radius(&GUI_Style__class_5dd3lX6oAKILzH__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_5dd3lX6oAKILzH__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_5dd3lX6oAKILzH__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_5dd3lX6oAKILzH__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_5dd3lX6oAKILzH__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_5dd3lX6oAKILzH__, 0);
  lv_style_set_pad_column(&GUI_Style__class_5dd3lX6oAKILzH__, 0);
  lv_style_set_layout(&GUI_Style__class_5dd3lX6oAKILzH__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_WOdzLKOUWTKn2D__);
  lv_style_set_text_color(&GUI_Style__class_WOdzLKOUWTKn2D__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_WOdzLKOUWTKn2D__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_WOdzLKOUWTKn2D__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_WOdzLKOUWTKn2D__, 0);
  lv_style_set_text_align(&GUI_Style__class_WOdzLKOUWTKn2D__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_PWig0wvDygIn92__);
  lv_style_set_bg_color(&GUI_Style__class_PWig0wvDygIn92__,
                        lv_color_make(180, 1, 1));
  lv_style_set_bg_opa(&GUI_Style__class_PWig0wvDygIn92__, 255);
  lv_style_set_radius(&GUI_Style__class_PWig0wvDygIn92__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_PWig0wvDygIn92__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_PWig0wvDygIn92__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_PWig0wvDygIn92__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_PWig0wvDygIn92__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_PWig0wvDygIn92__, 0);
  lv_style_set_pad_column(&GUI_Style__class_PWig0wvDygIn92__, 0);
  lv_style_set_layout(&GUI_Style__class_PWig0wvDygIn92__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_zIG6cz2FsG6fjL__);
  lv_style_set_text_color(&GUI_Style__class_zIG6cz2FsG6fjL__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_zIG6cz2FsG6fjL__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_zIG6cz2FsG6fjL__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_zIG6cz2FsG6fjL__, 0);
  lv_style_set_text_align(&GUI_Style__class_zIG6cz2FsG6fjL__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_GGJhSwdaT2bCHt__);
  lv_style_set_bg_color(&GUI_Style__class_GGJhSwdaT2bCHt__,
                        lv_color_make(95, 95, 95));
  lv_style_set_bg_opa(&GUI_Style__class_GGJhSwdaT2bCHt__, 255);
  lv_style_set_radius(&GUI_Style__class_GGJhSwdaT2bCHt__, 0);

  lv_style_init(&GUI_Style__class_bvAupbhGzO9lgK__);
  lv_style_set_min_width(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_max_width(&GUI_Style__class_bvAupbhGzO9lgK__, 8000);
  lv_style_set_min_height(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_max_height(&GUI_Style__class_bvAupbhGzO9lgK__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_bvAupbhGzO9lgK__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_bvAupbhGzO9lgK__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_bvAupbhGzO9lgK__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_bvAupbhGzO9lgK__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_bvAupbhGzO9lgK__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_bvAupbhGzO9lgK__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_pad_column(&GUI_Style__class_bvAupbhGzO9lgK__, 0);
  lv_style_set_layout(&GUI_Style__class_bvAupbhGzO9lgK__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_BMQiIJTpQecj2H__);
  lv_style_set_bg_color(&GUI_Style__class_BMQiIJTpQecj2H__,
                        lv_color_make(0, 0, 0));
  lv_style_set_bg_opa(&GUI_Style__class_BMQiIJTpQecj2H__, 255);
  lv_style_set_radius(&GUI_Style__class_BMQiIJTpQecj2H__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_BMQiIJTpQecj2H__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_BMQiIJTpQecj2H__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_BMQiIJTpQecj2H__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_BMQiIJTpQecj2H__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_BMQiIJTpQecj2H__, 10);
  lv_style_set_pad_column(&GUI_Style__class_BMQiIJTpQecj2H__, 0);
  lv_style_set_layout(&GUI_Style__class_BMQiIJTpQecj2H__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_qbslh11Av1hMuq__);
  lv_style_set_flex_flow(&GUI_Style__class_qbslh11Av1hMuq__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_qbslh11Av1hMuq__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_qbslh11Av1hMuq__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_qbslh11Av1hMuq__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_qbslh11Av1hMuq__, 10);
  lv_style_set_pad_column(&GUI_Style__class_qbslh11Av1hMuq__, 0);
  lv_style_set_layout(&GUI_Style__class_qbslh11Av1hMuq__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_aZS4mzVqpqteVv__);
  lv_style_set_text_font(&GUI_Style__class_aZS4mzVqpqteVv__, &title_1);
  lv_style_set_text_color(&GUI_Style__class_aZS4mzVqpqteVv__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_aZS4mzVqpqteVv__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_aZS4mzVqpqteVv__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_aZS4mzVqpqteVv__, 0);
  lv_style_set_text_align(&GUI_Style__class_aZS4mzVqpqteVv__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_flex_flow(&GUI_Style__class_aZS4mzVqpqteVv__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_aZS4mzVqpqteVv__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_aZS4mzVqpqteVv__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_aZS4mzVqpqteVv__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_aZS4mzVqpqteVv__, 0);
  lv_style_set_pad_column(&GUI_Style__class_aZS4mzVqpqteVv__, 0);
  lv_style_set_layout(&GUI_Style__class_aZS4mzVqpqteVv__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_Sew7zZBd2dEAC5__);
  lv_style_set_text_color(&GUI_Style__class_Sew7zZBd2dEAC5__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_Sew7zZBd2dEAC5__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_Sew7zZBd2dEAC5__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_Sew7zZBd2dEAC5__, 0);
  lv_style_set_text_align(&GUI_Style__class_Sew7zZBd2dEAC5__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_flex_flow(&GUI_Style__class_Sew7zZBd2dEAC5__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_Sew7zZBd2dEAC5__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_Sew7zZBd2dEAC5__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_Sew7zZBd2dEAC5__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_Sew7zZBd2dEAC5__, 0);
  lv_style_set_pad_column(&GUI_Style__class_Sew7zZBd2dEAC5__, 0);
  lv_style_set_layout(&GUI_Style__class_Sew7zZBd2dEAC5__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_4hGM1DOyWzKXGG__);
  lv_style_set_bg_color(&GUI_Style__class_4hGM1DOyWzKXGG__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_4hGM1DOyWzKXGG__, 255);
  lv_style_set_radius(&GUI_Style__class_4hGM1DOyWzKXGG__, 5);

  lv_style_init(&GUI_Style__class_HK0JrRlgfa8ZRB__);
  lv_style_set_text_color(&GUI_Style__class_HK0JrRlgfa8ZRB__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_HK0JrRlgfa8ZRB__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_HK0JrRlgfa8ZRB__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_HK0JrRlgfa8ZRB__, 0);
  lv_style_set_text_align(&GUI_Style__class_HK0JrRlgfa8ZRB__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_JAL8G2SBlbgsMK__);
  lv_style_set_image_recolor(&GUI_Style__class_JAL8G2SBlbgsMK__,
                             lv_color_make(0, 0, 0));
  lv_style_set_image_recolor_opa(&GUI_Style__class_JAL8G2SBlbgsMK__, 255);

  lv_style_init(&GUI_Style__class_kkoJh1hF2B6tCJ__);
  lv_style_set_outline_color(&GUI_Style__class_kkoJh1hF2B6tCJ__,
                             lv_color_make(0, 0, 0));
  lv_style_set_outline_opa(&GUI_Style__class_kkoJh1hF2B6tCJ__, 255);
  lv_style_set_outline_width(&GUI_Style__class_kkoJh1hF2B6tCJ__, 2);
  lv_style_set_outline_pad(&GUI_Style__class_kkoJh1hF2B6tCJ__, 0);
  lv_style_set_image_recolor(&GUI_Style__class_kkoJh1hF2B6tCJ__,
                             lv_color_make(138, 239, 44));
  lv_style_set_image_recolor_opa(&GUI_Style__class_kkoJh1hF2B6tCJ__, 255);
  lv_style_set_bg_color(&GUI_Style__class_kkoJh1hF2B6tCJ__,
                        lv_color_make(47, 72, 118));
  lv_style_set_bg_opa(&GUI_Style__class_kkoJh1hF2B6tCJ__, 255);
  lv_style_set_radius(&GUI_Style__class_kkoJh1hF2B6tCJ__, 5);

  lv_style_init(&GUI_Style__class_GBcuAJEHFeiSNf__);
  lv_style_set_text_letter_space(&GUI_Style__class_GBcuAJEHFeiSNf__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_GBcuAJEHFeiSNf__, 0);
  lv_style_set_text_align(&GUI_Style__class_GBcuAJEHFeiSNf__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_GBcuAJEHFeiSNf__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_GBcuAJEHFeiSNf__, 255);

  lv_style_init(&GUI_Style__class_QdPpiSKXWeZY8l__);
  lv_style_set_image_recolor(&GUI_Style__class_QdPpiSKXWeZY8l__,
                             lv_color_make(0, 0, 0));
  lv_style_set_image_recolor_opa(&GUI_Style__class_QdPpiSKXWeZY8l__, 255);

  lv_style_init(&GUI_Style__class_jKApx7l6HgsZwU__);
  lv_style_set_image_recolor(&GUI_Style__class_jKApx7l6HgsZwU__,
                             lv_color_make(138, 239, 44));
  lv_style_set_image_recolor_opa(&GUI_Style__class_jKApx7l6HgsZwU__, 255);
  lv_style_set_outline_color(&GUI_Style__class_jKApx7l6HgsZwU__,
                             lv_color_make(0, 0, 0));
  lv_style_set_outline_opa(&GUI_Style__class_jKApx7l6HgsZwU__, 255);
  lv_style_set_outline_pad(&GUI_Style__class_jKApx7l6HgsZwU__, 0);
  lv_style_set_outline_width(&GUI_Style__class_jKApx7l6HgsZwU__, 2);
  lv_style_set_bg_color(&GUI_Style__class_jKApx7l6HgsZwU__,
                        lv_color_make(1, 109, 17));
  lv_style_set_bg_opa(&GUI_Style__class_jKApx7l6HgsZwU__, 255);
  lv_style_set_radius(&GUI_Style__class_jKApx7l6HgsZwU__, 5);

  lv_style_init(&GUI_Style__class_5Eafbd6yvjyvsT__);
  lv_style_set_flex_flow(&GUI_Style__class_5Eafbd6yvjyvsT__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_5Eafbd6yvjyvsT__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_5Eafbd6yvjyvsT__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_5Eafbd6yvjyvsT__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_5Eafbd6yvjyvsT__, 0);
  lv_style_set_pad_column(&GUI_Style__class_5Eafbd6yvjyvsT__, 15);
  lv_style_set_layout(&GUI_Style__class_5Eafbd6yvjyvsT__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_Vtp3KvKLKtwz05__);
  lv_style_set_text_letter_space(&GUI_Style__class_Vtp3KvKLKtwz05__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_Vtp3KvKLKtwz05__, 0);
  lv_style_set_text_align(&GUI_Style__class_Vtp3KvKLKtwz05__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_Vtp3KvKLKtwz05__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_Vtp3KvKLKtwz05__, 255);

  lv_style_init(&GUI_Style__class_IehN4pBvRaLoqF__);
  lv_style_set_flex_flow(&GUI_Style__class_IehN4pBvRaLoqF__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_IehN4pBvRaLoqF__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_IehN4pBvRaLoqF__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_IehN4pBvRaLoqF__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_IehN4pBvRaLoqF__, 0);
  lv_style_set_pad_column(&GUI_Style__class_IehN4pBvRaLoqF__, 15);
  lv_style_set_layout(&GUI_Style__class_IehN4pBvRaLoqF__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_UsTxBsVMWpTM8Y__);
  lv_style_set_text_letter_space(&GUI_Style__class_UsTxBsVMWpTM8Y__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_UsTxBsVMWpTM8Y__, 0);
  lv_style_set_text_align(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_UsTxBsVMWpTM8Y__, 255);
  lv_style_set_flex_flow(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_UsTxBsVMWpTM8Y__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_UsTxBsVMWpTM8Y__, 0);
  lv_style_set_pad_column(&GUI_Style__class_UsTxBsVMWpTM8Y__, 0);
  lv_style_set_layout(&GUI_Style__class_UsTxBsVMWpTM8Y__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_HRt1tUvdsVkBGo__);
  lv_style_set_flex_flow(&GUI_Style__class_HRt1tUvdsVkBGo__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_HRt1tUvdsVkBGo__,
                               LV_FLEX_ALIGN_END);
  lv_style_set_flex_track_place(&GUI_Style__class_HRt1tUvdsVkBGo__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_HRt1tUvdsVkBGo__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_HRt1tUvdsVkBGo__, 0);
  lv_style_set_pad_column(&GUI_Style__class_HRt1tUvdsVkBGo__, 0);
  lv_style_set_layout(&GUI_Style__class_HRt1tUvdsVkBGo__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_lot1J19xVfrTSM__);
  lv_style_set_flex_flow(&GUI_Style__class_lot1J19xVfrTSM__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_lot1J19xVfrTSM__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_lot1J19xVfrTSM__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_lot1J19xVfrTSM__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_lot1J19xVfrTSM__, 0);
  lv_style_set_pad_column(&GUI_Style__class_lot1J19xVfrTSM__, 0);

  lv_style_init(&GUI_Style__class_ponc57mppwwLBh__);
  lv_style_set_flex_flow(&GUI_Style__class_ponc57mppwwLBh__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_ponc57mppwwLBh__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_ponc57mppwwLBh__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_ponc57mppwwLBh__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_ponc57mppwwLBh__, 0);
  lv_style_set_pad_column(&GUI_Style__class_ponc57mppwwLBh__, 15);
  lv_style_set_layout(&GUI_Style__class_ponc57mppwwLBh__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_ewB9ltfpeonQfa__);
  lv_style_set_text_letter_space(&GUI_Style__class_ewB9ltfpeonQfa__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_ewB9ltfpeonQfa__, 0);
  lv_style_set_text_align(&GUI_Style__class_ewB9ltfpeonQfa__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_ewB9ltfpeonQfa__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_ewB9ltfpeonQfa__, 255);
  lv_style_set_flex_flow(&GUI_Style__class_ewB9ltfpeonQfa__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_ewB9ltfpeonQfa__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_ewB9ltfpeonQfa__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_ewB9ltfpeonQfa__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_ewB9ltfpeonQfa__, 0);
  lv_style_set_pad_column(&GUI_Style__class_ewB9ltfpeonQfa__, 0);
  lv_style_set_layout(&GUI_Style__class_ewB9ltfpeonQfa__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_q7eZlp9MO0EzKy__);
  lv_style_set_flex_flow(&GUI_Style__class_q7eZlp9MO0EzKy__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_q7eZlp9MO0EzKy__,
                               LV_FLEX_ALIGN_END);
  lv_style_set_flex_track_place(&GUI_Style__class_q7eZlp9MO0EzKy__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_q7eZlp9MO0EzKy__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_q7eZlp9MO0EzKy__, 0);
  lv_style_set_pad_column(&GUI_Style__class_q7eZlp9MO0EzKy__, 0);
  lv_style_set_layout(&GUI_Style__class_q7eZlp9MO0EzKy__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_AIIBPtShI6bHVL__);
  lv_style_set_flex_flow(&GUI_Style__class_AIIBPtShI6bHVL__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_AIIBPtShI6bHVL__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_AIIBPtShI6bHVL__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_AIIBPtShI6bHVL__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_AIIBPtShI6bHVL__, 0);
  lv_style_set_pad_column(&GUI_Style__class_AIIBPtShI6bHVL__, 0);

  lv_style_init(&GUI_Style__class_UWMNsTuzutTPfW__);
  lv_style_set_flex_flow(&GUI_Style__class_UWMNsTuzutTPfW__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_UWMNsTuzutTPfW__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_UWMNsTuzutTPfW__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_UWMNsTuzutTPfW__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_UWMNsTuzutTPfW__, 0);
  lv_style_set_pad_column(&GUI_Style__class_UWMNsTuzutTPfW__, 15);
  lv_style_set_layout(&GUI_Style__class_UWMNsTuzutTPfW__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_Pxj4bSx4XPxr9u__);
  lv_style_set_text_letter_space(&GUI_Style__class_Pxj4bSx4XPxr9u__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_Pxj4bSx4XPxr9u__, 0);
  lv_style_set_text_align(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                          LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                          lv_color_make(155, 155, 155));
  lv_style_set_text_opa(&GUI_Style__class_Pxj4bSx4XPxr9u__, 255);
  lv_style_set_flex_flow(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_Pxj4bSx4XPxr9u__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_pad_row(&GUI_Style__class_Pxj4bSx4XPxr9u__, 0);
  lv_style_set_pad_column(&GUI_Style__class_Pxj4bSx4XPxr9u__, 0);
  lv_style_set_layout(&GUI_Style__class_Pxj4bSx4XPxr9u__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_F0bYoBMAkxnmi5__);
  lv_style_set_flex_flow(&GUI_Style__class_F0bYoBMAkxnmi5__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_F0bYoBMAkxnmi5__,
                               LV_FLEX_ALIGN_END);
  lv_style_set_flex_track_place(&GUI_Style__class_F0bYoBMAkxnmi5__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_F0bYoBMAkxnmi5__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_F0bYoBMAkxnmi5__, 0);
  lv_style_set_pad_column(&GUI_Style__class_F0bYoBMAkxnmi5__, 0);
  lv_style_set_layout(&GUI_Style__class_F0bYoBMAkxnmi5__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_h8YvkrJbLCkkxu__);
  lv_style_set_flex_flow(&GUI_Style__class_h8YvkrJbLCkkxu__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_h8YvkrJbLCkkxu__,
                               LV_FLEX_ALIGN_START);
  lv_style_set_flex_track_place(&GUI_Style__class_h8YvkrJbLCkkxu__,
                                LV_FLEX_ALIGN_START);
  lv_style_set_flex_cross_place(&GUI_Style__class_h8YvkrJbLCkkxu__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_h8YvkrJbLCkkxu__, 0);
  lv_style_set_pad_column(&GUI_Style__class_h8YvkrJbLCkkxu__, 0);

  lv_style_init(&GUI_Style__class_P0ckvf5Cd2Mzgn__);
  lv_style_set_flex_flow(&GUI_Style__class_P0ckvf5Cd2Mzgn__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_P0ckvf5Cd2Mzgn__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_P0ckvf5Cd2Mzgn__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_P0ckvf5Cd2Mzgn__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_P0ckvf5Cd2Mzgn__, 0);
  lv_style_set_pad_column(&GUI_Style__class_P0ckvf5Cd2Mzgn__, 30);
  lv_style_set_layout(&GUI_Style__class_P0ckvf5Cd2Mzgn__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_8giE6vbQ8nbm6U__);
  lv_style_set_bg_color(&GUI_Style__class_8giE6vbQ8nbm6U__,
                        lv_color_make(0, 125, 48));
  lv_style_set_bg_opa(&GUI_Style__class_8giE6vbQ8nbm6U__, 255);
  lv_style_set_radius(&GUI_Style__class_8giE6vbQ8nbm6U__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_8giE6vbQ8nbm6U__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_8giE6vbQ8nbm6U__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_8giE6vbQ8nbm6U__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_8giE6vbQ8nbm6U__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_8giE6vbQ8nbm6U__, 0);
  lv_style_set_pad_column(&GUI_Style__class_8giE6vbQ8nbm6U__, 0);
  lv_style_set_layout(&GUI_Style__class_8giE6vbQ8nbm6U__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_TkoVT5AI0Y9ewe__);
  lv_style_set_text_color(&GUI_Style__class_TkoVT5AI0Y9ewe__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_TkoVT5AI0Y9ewe__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_TkoVT5AI0Y9ewe__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_TkoVT5AI0Y9ewe__, 0);
  lv_style_set_text_align(&GUI_Style__class_TkoVT5AI0Y9ewe__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_jiEaq40fppn8N1__);
  lv_style_set_bg_color(&GUI_Style__class_jiEaq40fppn8N1__,
                        lv_color_make(180, 1, 1));
  lv_style_set_bg_opa(&GUI_Style__class_jiEaq40fppn8N1__, 255);
  lv_style_set_radius(&GUI_Style__class_jiEaq40fppn8N1__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_jiEaq40fppn8N1__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_jiEaq40fppn8N1__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_jiEaq40fppn8N1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_jiEaq40fppn8N1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_jiEaq40fppn8N1__, 0);
  lv_style_set_pad_column(&GUI_Style__class_jiEaq40fppn8N1__, 0);
  lv_style_set_layout(&GUI_Style__class_jiEaq40fppn8N1__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_yL8O2Ek0kq8lRL__);
  lv_style_set_text_color(&GUI_Style__class_yL8O2Ek0kq8lRL__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_yL8O2Ek0kq8lRL__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_yL8O2Ek0kq8lRL__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_yL8O2Ek0kq8lRL__, 0);
  lv_style_set_text_align(&GUI_Style__class_yL8O2Ek0kq8lRL__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_O3vmtiU810aw4c__);
  lv_style_set_bg_color(&GUI_Style__class_O3vmtiU810aw4c__,
                        lv_color_make(95, 95, 95));
  lv_style_set_bg_opa(&GUI_Style__class_O3vmtiU810aw4c__, 255);
  lv_style_set_radius(&GUI_Style__class_O3vmtiU810aw4c__, 0);

  lv_style_init(&GUI_Style__class_DQ4kt4qPpym6rl__);
  lv_style_set_min_width(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_max_width(&GUI_Style__class_DQ4kt4qPpym6rl__, 8000);
  lv_style_set_min_height(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_max_height(&GUI_Style__class_DQ4kt4qPpym6rl__, 8000);
  lv_style_set_transform_scale_x(&GUI_Style__class_DQ4kt4qPpym6rl__, 256);
  lv_style_set_transform_scale_y(&GUI_Style__class_DQ4kt4qPpym6rl__, 256);
  lv_style_set_transform_rotation(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_transform_pivot_x(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_transform_pivot_y(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_flex_flow(&GUI_Style__class_DQ4kt4qPpym6rl__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_DQ4kt4qPpym6rl__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_DQ4kt4qPpym6rl__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_DQ4kt4qPpym6rl__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_pad_column(&GUI_Style__class_DQ4kt4qPpym6rl__, 0);
  lv_style_set_layout(&GUI_Style__class_DQ4kt4qPpym6rl__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_kQXih6nLC1SrH3__);
  lv_style_set_bg_color(&GUI_Style__class_kQXih6nLC1SrH3__,
                        lv_color_make(19, 17, 22));
  lv_style_set_bg_opa(&GUI_Style__class_kQXih6nLC1SrH3__, 255);
  lv_style_set_radius(&GUI_Style__class_kQXih6nLC1SrH3__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_kQXih6nLC1SrH3__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_kQXih6nLC1SrH3__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_kQXih6nLC1SrH3__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_kQXih6nLC1SrH3__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_kQXih6nLC1SrH3__, 10);
  lv_style_set_pad_column(&GUI_Style__class_kQXih6nLC1SrH3__, 0);
  lv_style_set_layout(&GUI_Style__class_kQXih6nLC1SrH3__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_0LBWzZLzLfqETo__);
  lv_style_set_text_font(&GUI_Style__class_0LBWzZLzLfqETo__, &title_1);
  lv_style_set_text_color(&GUI_Style__class_0LBWzZLzLfqETo__,
                          lv_color_make(255, 255, 255));
  lv_style_set_text_opa(&GUI_Style__class_0LBWzZLzLfqETo__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_0LBWzZLzLfqETo__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_0LBWzZLzLfqETo__, 0);
  lv_style_set_text_align(&GUI_Style__class_0LBWzZLzLfqETo__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_auBrxRjHQbK0ZC__);
  lv_style_set_bg_color(&GUI_Style__class_auBrxRjHQbK0ZC__,
                        lv_color_make(255, 255, 255));
  lv_style_set_bg_opa(&GUI_Style__class_auBrxRjHQbK0ZC__, 255);
  lv_style_set_radius(&GUI_Style__class_auBrxRjHQbK0ZC__, 5);

  lv_style_init(&GUI_Style__class_kzXqB1csdmBWuI__);
  lv_style_set_outline_color(&GUI_Style__class_kzXqB1csdmBWuI__,
                             lv_color_make(0, 0, 0));
  lv_style_set_outline_opa(&GUI_Style__class_kzXqB1csdmBWuI__, 255);
  lv_style_set_outline_width(&GUI_Style__class_kzXqB1csdmBWuI__, 2);
  lv_style_set_outline_pad(&GUI_Style__class_kzXqB1csdmBWuI__, 0);

  lv_style_init(&GUI_Style__class_ddJcy0PZRiVhu1__);
  lv_style_set_flex_flow(&GUI_Style__class_ddJcy0PZRiVhu1__, LV_FLEX_FLOW_ROW);
  lv_style_set_flex_main_place(&GUI_Style__class_ddJcy0PZRiVhu1__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_ddJcy0PZRiVhu1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_ddJcy0PZRiVhu1__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_ddJcy0PZRiVhu1__, 0);
  lv_style_set_pad_column(&GUI_Style__class_ddJcy0PZRiVhu1__, 30);
  lv_style_set_layout(&GUI_Style__class_ddJcy0PZRiVhu1__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_1aTDzshajO9dKi__);
  lv_style_set_bg_color(&GUI_Style__class_1aTDzshajO9dKi__,
                        lv_color_make(0, 125, 48));
  lv_style_set_bg_opa(&GUI_Style__class_1aTDzshajO9dKi__, 255);
  lv_style_set_radius(&GUI_Style__class_1aTDzshajO9dKi__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_1aTDzshajO9dKi__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_1aTDzshajO9dKi__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_1aTDzshajO9dKi__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_1aTDzshajO9dKi__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_1aTDzshajO9dKi__, 0);
  lv_style_set_pad_column(&GUI_Style__class_1aTDzshajO9dKi__, 0);
  lv_style_set_layout(&GUI_Style__class_1aTDzshajO9dKi__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_bH6lQLxPISysIy__);
  lv_style_set_text_color(&GUI_Style__class_bH6lQLxPISysIy__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_bH6lQLxPISysIy__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_bH6lQLxPISysIy__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_bH6lQLxPISysIy__, 0);
  lv_style_set_text_align(&GUI_Style__class_bH6lQLxPISysIy__,
                          LV_TEXT_ALIGN_LEFT);

  lv_style_init(&GUI_Style__class_UfvGqtj4LNg70F__);
  lv_style_set_bg_color(&GUI_Style__class_UfvGqtj4LNg70F__,
                        lv_color_make(180, 1, 1));
  lv_style_set_bg_opa(&GUI_Style__class_UfvGqtj4LNg70F__, 255);
  lv_style_set_radius(&GUI_Style__class_UfvGqtj4LNg70F__, 5);
  lv_style_set_flex_flow(&GUI_Style__class_UfvGqtj4LNg70F__,
                         LV_FLEX_FLOW_COLUMN);
  lv_style_set_flex_main_place(&GUI_Style__class_UfvGqtj4LNg70F__,
                               LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_track_place(&GUI_Style__class_UfvGqtj4LNg70F__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_flex_cross_place(&GUI_Style__class_UfvGqtj4LNg70F__,
                                LV_FLEX_ALIGN_CENTER);
  lv_style_set_pad_row(&GUI_Style__class_UfvGqtj4LNg70F__, 0);
  lv_style_set_pad_column(&GUI_Style__class_UfvGqtj4LNg70F__, 0);
  lv_style_set_layout(&GUI_Style__class_UfvGqtj4LNg70F__, LV_LAYOUT_FLEX);

  lv_style_init(&GUI_Style__class_xt6pGh28xe2gYG__);
  lv_style_set_text_color(&GUI_Style__class_xt6pGh28xe2gYG__,
                          lv_color_make(0, 0, 0));
  lv_style_set_text_opa(&GUI_Style__class_xt6pGh28xe2gYG__, 255);
  lv_style_set_text_letter_space(&GUI_Style__class_xt6pGh28xe2gYG__, 0);
  lv_style_set_text_line_space(&GUI_Style__class_xt6pGh28xe2gYG__, 0);
  lv_style_set_text_align(&GUI_Style__class_xt6pGh28xe2gYG__,
                          LV_TEXT_ALIGN_LEFT);
}
