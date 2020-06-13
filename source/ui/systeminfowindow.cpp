#include "systeminfowindow.h"

#include "lv_disp.h"
#include "lv_label.h"
#include "lv_page.h"
#include <sstream>
#include "gcc_timestamp.h"
#include "lv_version.h"
#include "mbed_version.h"

SystemInfoWindow::SystemInfoWindow() : 
    BaseWindow(),
    pgInfo(nullptr),
    lblTitle(nullptr),
    txt()
{
    lblTitle = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_roboto22;
    lv_style_copy(&lbl_style_roboto22, &lv_style_plain);
    lbl_style_roboto22.text.font = &lv_font_roboto_22;  //Set a different font
    lv_label_set_style(lblTitle, LV_LABEL_STYLE_MAIN, &lbl_style_roboto22);
	lv_obj_align(lblTitle, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);  //Align to the top
	lv_obj_set_hidden(lblTitle, false);
    lv_label_set_text(lblTitle, "System info");


    pgInfo = lv_page_create(lv_scr_act(), NULL);
    lv_obj_set_size(pgInfo, width(), height()-60);
	lv_obj_align(pgInfo, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_hidden(pgInfo, false);

    static lv_style_t style_bg;
    lv_style_copy(&style_bg, &lv_style_plain);
    style_bg.body.main_color = LV_COLOR_WHITE;
    style_bg.body.grad_color = LV_COLOR_WHITE;
    style_bg.body.border.color = LV_COLOR_BLACK;
    style_bg.body.border.width = 1;
    style_bg.body.border.part = LV_BORDER_TOP;
    style_bg.body.radius = 0;
    style_bg.body.padding.right = 0;
    style_bg.body.padding.bottom = 0;
    style_bg.body.padding.top = 0;
    style_bg.body.padding.left = 0;
    lv_page_set_style(pgInfo, LV_PAGE_STYLE_BG, &style_bg);

    static lv_style_t style_scrl;
    lv_style_copy(&style_scrl, &lv_style_plain);
    style_scrl.body.main_color = LV_COLOR_WHITE;
    style_scrl.body.grad_color = LV_COLOR_WHITE;
    style_scrl.body.radius = 0;
    lv_page_set_style(pgInfo, LV_PAGE_STYLE_SCRL, &style_scrl);

    lv_page_set_sb_mode(pgInfo, LV_SB_MODE_OFF);

    std::ostringstream stringStream;
    stringStream << "#cccccc -- Hardware --#" << std::endl;
    stringStream << "SOC: NORDIC nRF52832" << std::endl;
    stringStream << "Clock: " << (SystemCoreClock/1000000) << "MHz" << std::endl;
    stringStream << "#cccccc ---- PineTime-OS --#" << std::endl;
    stringStream << "Version: v0.0.1" << std::endl;
    stringStream << "Build: " << UNIX_TIMESTAMP << std::endl;
    stringStream << "#cccccc ---- Libraries --#" << std::endl;
    stringStream << "LVGL: " << LVGL_VERSION_MAJOR << "." <<  LVGL_VERSION_MINOR << "." << LVGL_VERSION_PATCH << std::endl;
    stringStream << "mbed-os: " << MBED_MAJOR_VERSION << "." <<  MBED_MINOR_VERSION << "." << MBED_PATCH_VERSION << std::endl;
    txt = stringStream.str();

    lv_obj_t * label = lv_label_create(pgInfo, NULL);
    lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);            //Automatically break long lines
    lv_obj_set_width(label, lv_page_get_fit_width(pgInfo));        //Set the label width to max value to not show hor. scroll bars
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, txt.c_str());
}

SystemInfoWindow::~SystemInfoWindow()
{
    lv_obj_del(pgInfo);
    lv_obj_del(lblTitle);
}