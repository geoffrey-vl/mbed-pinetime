#include "configwatchfacewindow.h"

#include "lv_disp.h"
#include "lv_ddlist.h"
#include <string>

static void my_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        ConfigWatchFaceWindow* window = ((ConfigWatchFaceWindow*)lv_obj_get_user_data(obj));
        window->onItemSelected();
    }
}

ConfigWatchFaceWindow::ConfigWatchFaceWindow() : 
    BaseWindow(),
    lstClocks(nullptr),
    mSelectedItem(0)
{
    lblTitle = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_roboto22;
    lv_style_copy(&lbl_style_roboto22, &lv_style_plain);
    lbl_style_roboto22.text.font = &lv_font_roboto_22;  //Set a different font
    lv_label_set_style(lblTitle, LV_LABEL_STYLE_MAIN, &lbl_style_roboto22);
	lv_obj_align(lblTitle, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);  //Align to the top
	lv_obj_set_hidden(lblTitle, false);
    lv_label_set_text(lblTitle, "Watch faces");

    lstClocks = lv_ddlist_create(lv_scr_act(), NULL);
	lv_obj_align(lstClocks, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_hidden(lstClocks, false);
    std::string options = "Analog\nDigital";
    lv_ddlist_set_options(lstClocks, options.c_str());
    lv_ddlist_set_selected(lstClocks, 0);
    lv_ddlist_open(lstClocks, LV_ANIM_OFF);
    lv_ddlist_set_stay_open(lstClocks, true);
    lv_ddlist_set_fix_width(lstClocks, width());
    //link this class instance to lvgl object
    // We need it to get our instance inside event callbacks
    lv_obj_set_user_data(lstClocks, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(lstClocks, my_event_cb);   //Assign an event callback
}

ConfigWatchFaceWindow::~ConfigWatchFaceWindow()
{
    lv_obj_del(lstClocks);
    lv_obj_del(lblTitle);
}

void ConfigWatchFaceWindow::onItemSelected()
{
    mSelectedItem = lv_ddlist_get_selected(lstClocks);
    signalFinished();
}