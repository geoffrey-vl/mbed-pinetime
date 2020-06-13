#include "configurationwindow.h"

#include "lv_disp.h"
#include "lv_list.h"
#include <string>

static void my_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        //int32_t selectedItem = lv_list_get_btn_index(nullptr, obj);
        //ConfigurationWindow* window = ((ConfigurationWindow*)lv_obj_get_user_data(obj));
        //window->onItemSelected(selectedItem);
    }
}

ConfigurationWindow::ConfigurationWindow() : 
    BaseWindow(),
    lstSubmenu(nullptr),
    lblTitle(nullptr),
    mSelectedItem(0)
{
    lblTitle = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_roboto22;
    lv_style_copy(&lbl_style_roboto22, &lv_style_plain);
    lbl_style_roboto22.text.font = &lv_font_roboto_22;  //Set a different font
    lv_label_set_style(lblTitle, LV_LABEL_STYLE_MAIN, &lbl_style_roboto22);
	lv_obj_align(lblTitle, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);  //Align to the top
	lv_obj_set_hidden(lblTitle, false);
    lv_label_set_text(lblTitle, "Settings");

    lstSubmenu = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(lstSubmenu, width(), height()-60);
	lv_obj_align(lstSubmenu, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_hidden(lstSubmenu, false);
    static lv_style_t lstBtnBgStyle;
    lv_style_copy(&lstBtnBgStyle, &lv_style_plain);
    lstBtnBgStyle.body.grad_color = LV_COLOR_WHITE;
    lstBtnBgStyle.body.main_color = LV_COLOR_WHITE;
    lstBtnBgStyle.body.radius = 0;
    lstBtnBgStyle.body.padding.left = 0;
    lstBtnBgStyle.body.padding.right = 0;
    lstBtnBgStyle.body.border.part = LV_BORDER_TOP;
    lstBtnBgStyle.body.border.width = 1;
    lv_list_set_style(lstSubmenu, LV_LIST_STYLE_BG, &lstBtnBgStyle); 
    static lv_style_t lstBtnRelStyle;
    lv_style_copy(&lstBtnRelStyle, lv_list_get_style(lstSubmenu, LV_LIST_STYLE_BTN_REL));
    lstBtnRelStyle.body.padding.left = 20;
    lstBtnRelStyle.body.padding.right = 0;
    lstBtnRelStyle.body.padding.top = 10;
    lstBtnRelStyle.body.padding.bottom = 10;
    lstBtnRelStyle.body.radius = 0;
    lstBtnRelStyle.body.border.part = LV_BORDER_NONE;
    lv_list_set_style(lstSubmenu, LV_LIST_STYLE_BTN_REL, &lstBtnRelStyle);
    static lv_style_t lstBtnPrStyle;
    lv_style_copy(&lstBtnPrStyle, lv_list_get_style(lstSubmenu, LV_LIST_STYLE_BTN_PR));
    lstBtnPrStyle.body.padding.left = 20; 
    lstBtnPrStyle.body.padding.right = 0;
    lstBtnPrStyle.body.padding.top = 10;
    lstBtnPrStyle.body.padding.bottom = 10;
    lstBtnPrStyle.body.radius = 0;
    lstBtnPrStyle.body.border.part = LV_BORDER_NONE;
    lv_list_set_style(lstSubmenu, LV_LIST_STYLE_BTN_PR, &lstBtnPrStyle);

    lv_list_set_sb_mode(lstSubmenu, LV_SB_MODE_OFF);
    //add menu entries, and add this class instance to lvgl object.
    // We need it to get our instance inside event callbacks
    lv_obj_t* btn = nullptr;
    btn = lv_list_add_btn(lstSubmenu, nullptr, "Time");
    lv_obj_set_user_data(btn, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(btn, my_event_cb);
    btn = lv_list_add_btn(lstSubmenu, nullptr, "Date");
    lv_obj_set_user_data(btn, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(btn, my_event_cb);
    btn = lv_list_add_btn(lstSubmenu, nullptr, "Watch faces");
    lv_obj_set_user_data(btn, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(btn, my_event_cb);
    btn = lv_list_add_btn(lstSubmenu, nullptr, "System info");
    lv_obj_set_user_data(btn, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(btn, my_event_cb);
    btn = lv_list_add_btn(lstSubmenu, LV_SYMBOL_LEFT, "Back");
    lv_obj_set_user_data(btn, (lv_obj_user_data_t)this); 
    lv_obj_set_event_cb(btn, my_event_cb);
}

ConfigurationWindow::~ConfigurationWindow()
{
    lv_obj_del(lstSubmenu);
    lv_obj_del(lblTitle);
}

void ConfigurationWindow::onItemSelected(int32_t selectedItem)
{
    mSelectedItem = selectedItem;
    //signalFinished();
}