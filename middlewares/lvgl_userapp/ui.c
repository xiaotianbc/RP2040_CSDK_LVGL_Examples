// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: SquareLine_Project

#include <stdio.h>
#include <pico/types.h>
#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_Screen1_Label1;
lv_obj_t *ui_Screen1_Label2;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP != 1
#error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
static uint8_t charbufs[128];

///////////////////// FUNCTIONS ////////////////////
void my_timer(lv_timer_t *timer) {
    /*Use the user_data*/
    uint32_t *user_data = timer->user_data;
    static uint cnt = 0;
    /*Do something with LVGL*/
    sprintf(charbufs, "timer:%d\n", cnt++);
    lv_label_set_text(ui_Screen1_Label2, charbufs);
}

static uint32_t user_data = 10;
lv_timer_t *timer;

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void) {
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Screen1_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Screen1_Label1, "Hello, world");

    ui_Screen1_Label2 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Label2, 0);
    lv_obj_set_y(ui_Screen1_Label2, 28);
    lv_obj_set_align(ui_Screen1_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Screen1_Label2, "second line");
    timer = lv_timer_create(my_timer, 500, &user_data);
}

void ui_init(void) {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    lv_disp_load_scr(ui_Screen1);
}
