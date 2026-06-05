#include "../GUI.h"

void GUI_initScreen__screen() {
  GUI_Screen__screen = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__screen, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Image__screen__image = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_1 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_1, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_1, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_1, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_2 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_2, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_2, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_2, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_2, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_3 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_3, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_3, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_3, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_4 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_4, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_4, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_4, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_4, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_5 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_5, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_5, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_5, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_5, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_6 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_6, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_6, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_6, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_6, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_7 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_7, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_7, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_7, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_7, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_8 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_8, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_8, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_8, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_8, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_9 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_9, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_9, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_9, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_9, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_10 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_10, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_10, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_10, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_10, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_11 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_11, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_11, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_11, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_11, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_12 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_12, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_12, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_12, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_12, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_13 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_13, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_13, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_13, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_13, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_14 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_14, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_14, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_14, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_14, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_15 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_15, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_15, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_15, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_15, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_16 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_16, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_16, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_16, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_16, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_17 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_17, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_17, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_17, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_17, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_18 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_18, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_18, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_18, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_18, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_19 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_19, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_19, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_19, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_19, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_20 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_20, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_20, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_20, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_20, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_21 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_21, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_21, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_21, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_21, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_22 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_22, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_22, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_22, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_22, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_23 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_23, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_23, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_23, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_23, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_24 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_24, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_24, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_24, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_24, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_25 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_25, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_25, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_25, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_25, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_26 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_26, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_26, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_26, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_26, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_27 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_27, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_27, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_27, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_27, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_28 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_28, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_28, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_28, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_28, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_29 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_29, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_29, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_29, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_29, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_30 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_30, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_30, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_30, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_30, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_31 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_31, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_31, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_31, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_31, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_32 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_32, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_32, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_32, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_32, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__screen__image_33 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_33, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_33, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_33, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_33, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_37 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_37, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_37, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_37, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_37, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_38 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_38, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_38, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_38, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_38, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Image__screen__image_40 = lv_image_create(GUI_Screen__screen);
  lv_obj_add_flag(GUI_Image__screen__image_40, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__screen__image_40, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__screen__image_40, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__screen__image_40, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_initScreenStyles__screen();
  GUI_initScreenTexts__screen();
}

void GUI_initScreenTexts__screen() {
}

void GUI_initScreenStyles__screen() {
  lv_image_set_src(GUI_Image__screen__image,
                   &upload_networkred_45539a8ee52f46688e0fbeb4c45b9ddf_png);

  lv_image_set_src(GUI_Image__screen__image_1,
                   &upload_2s_37e4aa2fa7024542b513edb68eebbfa3_png);

  lv_image_set_src(GUI_Image__screen__image_2,
                   &upload_3s_e6f8aad40a204657b22d932c4b20fb18_png);

  lv_image_set_src(GUI_Image__screen__image_3,
                   &upload_7s_c385fe6874fa4da5853f367e29db7db4_png);

  lv_image_set_src(GUI_Image__screen__image_4,
                   &upload_7s_c385fe6874fa4da5853f367e29db7db4_png);

  lv_image_set_src(GUI_Image__screen__image_5,
                   &upload_11s_7c1fa968af52485bbeb874d89e56f96b_png);

  lv_image_set_src(GUI_Image__screen__image_6,
                   &upload_12s_7ed83dc3cb5f41f1a541dfd52ba15f5c_png);

  lv_image_set_src(GUI_Image__screen__image_7,
                   &upload_14s_b570241f67234745a56f9ddc06bd7800_png);

  lv_image_set_src(GUI_Image__screen__image_8,
                   &upload_15s_71a05d920e1a428e86707846a7f46127_png);

  lv_image_set_src(GUI_Image__screen__image_9,
                   &upload_18s_e55d60ec77034c32846327663b493f72_png);

  lv_image_set_src(GUI_Image__screen__image_10,
                   &upload_19s_c04d370ce95c403b82416b454729a8a8_png);

  lv_image_set_src(GUI_Image__screen__image_11,
                   &upload_22s_f739a4fff8ce46c5a09433c09b5b94c6_png);

  lv_image_set_src(GUI_Image__screen__image_12,
                   &upload_26s_731f337d30e945da969f8e1271adfa24_png);

  lv_image_set_src(GUI_Image__screen__image_13,
                   &upload_33s_b5e096b136ce4b6fbf7748e265b3d5eb_png);

  lv_image_set_src(GUI_Image__screen__image_14,
                   &upload_34s_d824cbffe8e8400cabc43e41608fd72d_png);

  lv_image_set_src(GUI_Image__screen__image_15,
                   &upload_35s_7a2b8dbb6e8a4c1d8a796fed8458d198_png);

  lv_image_set_src(GUI_Image__screen__image_16,
                   &upload_2_e2278bcef5404f3da02d7aa2a510f534_png);

  lv_image_set_src(GUI_Image__screen__image_17,
                   &upload_3_20748ad6e84045c396cda8b366aa5d07_png);

  lv_image_set_src(GUI_Image__screen__image_18,
                   &upload_7_5e6957ba671848fcae58518a4764438c_png);

  lv_image_set_src(GUI_Image__screen__image_19,
                   &upload_11_d928da2afce943088fc003fded319081_png);

  lv_image_set_src(GUI_Image__screen__image_20,
                   &upload_12_71141d8aba63422c8a7377026b8cecc6_png);

  lv_image_set_src(GUI_Image__screen__image_21,
                   &upload_14_14ff6d55212c4e7b945e173b90b81139_png);

  lv_image_set_src(GUI_Image__screen__image_22,
                   &upload_15_3a414dd3d7e14d559033812d417eaa90_png);

  lv_image_set_src(GUI_Image__screen__image_23,
                   &upload_18_b42c444b7f1445fc9003e4caa65d5a60_png);

  lv_image_set_src(GUI_Image__screen__image_24,
                   &upload_19_2425f3503fc04098b87dd50fc6604b61_png);

  lv_image_set_src(GUI_Image__screen__image_25,
                   &upload_22_6dccc7824e764e4a9b70ff8d5b078fad_png);

  lv_image_set_src(GUI_Image__screen__image_26,
                   &upload_26_28f0c2296c7742bbad7e6ed91f80b81a_png);

  lv_image_set_src(GUI_Image__screen__image_27,
                   &upload_33_b02a9c2486c94766bd9856f56db21f1f_png);

  lv_image_set_src(GUI_Image__screen__image_28,
                   &upload_34_4c27b41ae3a1496089c4d14141615cbd_png);

  lv_image_set_src(GUI_Image__screen__image_29,
                   &upload_35_cc7cb52862b9422aba85d07b935bc63c_png);

  lv_image_set_src(GUI_Image__screen__image_30,
                   &upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png);

  lv_image_set_src(GUI_Image__screen__image_31,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_image_set_src(GUI_Image__screen__image_32,
                   &upload_menusettings_ac03cce753704d6fa240fb7acbd315c1_png);

  lv_image_set_src(GUI_Image__screen__image_33,
                   &upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);

  lv_image_set_src(GUI_Image__screen__image_37,
                   &upload_networkred_45539a8ee52f46688e0fbeb4c45b9ddf_png);

  lv_image_set_src(GUI_Image__screen__image_38,
                   &upload_stopbutton_a37c756148194bc181010db779e72ea4_png);

  lv_image_set_src(GUI_Image__screen__image_40,
                   &upload_music_btn_red_410df9ce0fe84d7ea36d6e6554682ce8_png);
}
