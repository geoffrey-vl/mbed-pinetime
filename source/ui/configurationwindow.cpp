#include "configurationwindow.h"

#include "lv_disp.h"
#include "lv_ddlist.h"
#include <string>

static void my_event_cb(lv_obj_t * obj, lv_event_t event)
{
    switch(event) {
        case LV_EVENT_CLICKED:
            ConfigurationWindow* window = ((ConfigurationWindow*)lv_obj_get_user_data(obj));
            window->onItemSelected();
            break;
    }
}

ConfigurationWindow::ConfigurationWindow() : 
    BaseWindow(),
    lstClocks(nullptr),
    mSelectedItem(0)
{
    lblTitle = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(lblTitle, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);  //Align to the top
	lv_obj_set_hidden(lblTitle, false);
    lv_label_set_text(lblTitle, "Select clock type");

    lstClocks = lv_ddlist_create(lv_scr_act(), NULL);
	lv_obj_align(lstClocks, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_hidden(lstClocks, false);
    std::string options = "Analog\nDigital";
    lv_ddlist_set_options(lstClocks, options.c_str());
    lv_ddlist_set_selected(lstClocks, 1);
    lv_ddlist_open(lstClocks, LV_ANIM_OFF);
    lv_ddlist_set_stay_open(lstClocks, true);
    lv_ddlist_set_fix_width(lstClocks, width());
    //link this class instance to lvgl object
    // We need it to get our instance inside event callbacks
    lv_obj_set_user_data(lstClocks, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(lstClocks, my_event_cb);   //Assign an event callback
}

ConfigurationWindow::~ConfigurationWindow()
{
    lv_obj_del(lstClocks);
    lv_obj_del(lblTitle);
}

void ConfigurationWindow::onItemSelected()
{
    mSelectedItem = lv_ddlist_get_selected(lstClocks);
    signalFinished();
}