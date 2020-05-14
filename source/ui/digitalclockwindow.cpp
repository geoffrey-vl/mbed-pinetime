#include "digitalclockwindow.h"

#include "lv_disp.h"
#include "lv_label.h"

#include <iostream>
#include <ctime>

#include "peripherals.h"

#include "lv_fonts_pinetime.h"



DigitalClockWindow::DigitalClockWindow() : 
    BaseWindow(),
    icBatt(nullptr),
    lblBatt(nullptr),
    lblTime(nullptr),
    lblDay(nullptr),
    currentTime(time(0)),
    vbattpc(0),
    isCharging(false)
{
    // battery icon
    icBatt = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_pinetimefonts16;
    lv_style_copy(&lbl_style_pinetimefonts16, &lv_style_plain);
    lbl_style_pinetimefonts16.text.font = &lv_font_pinesymbols_16;  //Set a diferent font
    lv_label_set_style(icBatt, LV_LABEL_STYLE_MAIN, &lbl_style_pinetimefonts16);
	lv_obj_align(icBatt, NULL, LV_ALIGN_IN_TOP_RIGHT, -25, 0);  // Align to the top
	lv_obj_set_hidden(icBatt, false);
    
    // battery percentage label
    lblBatt = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(lblBatt, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 0);  // Align to the top
	lv_obj_set_hidden(lblBatt, false);
    //update battery variables and draw
    isCharging = Peripherals::batteryMonitor.isCharging();
    vbattpc = Peripherals::batteryMonitor.batteryPercentage();
    setTxtBattPc();

    // time label
	lblTime = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_roboto90;
    lv_style_copy(&lbl_style_roboto90, &lv_style_plain);
    lbl_style_roboto90.text.font = &lv_font_roboto_numbers_90;  //Set a different font
    lv_label_set_style(lblTime, LV_LABEL_STYLE_MAIN, &lbl_style_roboto90);
	lv_obj_align(lblTime, NULL, LV_ALIGN_IN_LEFT_MID, 4, 0);  //Align to the top
	lv_obj_set_hidden(lblTime, false);
    // date label
    lblDay = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(lblDay, NULL, LV_ALIGN_IN_BOTTOM_MID, -25, 0);  //Align to the top
	lv_obj_set_hidden(lblDay, false);
    // draw calendar date and time
    setTxtDateTime();
}

DigitalClockWindow::~DigitalClockWindow()
{
    lv_obj_del(lblDay);
    lv_obj_del(lblTime);
    lv_obj_del(lblBatt);
    lv_obj_del(icBatt);
}

void DigitalClockWindow::draw()
{
    //get new time, check charging feedback
    time_t now = time(0);
    bool isCurrentlyCharging = Peripherals::batteryMonitor.isCharging();

    // triggers to redraw parts of the UI:
    // - minute has passed
    // - charging state changed
    if(now > (currentTime + 60) || (isCurrentlyCharging != isCharging) ) 
    { 
        //redraw time and date
        currentTime = now;
        setTxtDateTime();

        // update battery monitor
        isCharging = isCurrentlyCharging;        
        vbattpc = Peripherals::batteryMonitor.batteryPercentage();
        setTxtBattPc();
    }

    BaseWindow::draw();
}

void DigitalClockWindow::setTxtDateTime()
{
    tm *localTm = localtime(&currentTime);
    lv_label_set_text_fmt(lblTime, "%02d:%02d", localTm->tm_hour, localTm->tm_min); 
    lv_label_set_text_fmt(lblDay, "%02d/%02d/%04d", (localTm->tm_mday), (localTm->tm_mon+1), (localTm->tm_year+1900)); 
}

void DigitalClockWindow::setTxtBattPc()
{
    lv_label_set_text_fmt(lblBatt, "%02d%%", vbattpc);

    if(isCharging)
        lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_CHARGING);
    else {
        if(vbattpc > 80)
            lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_FULL);
        else if(vbattpc > 60)
            lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_3);
        else if(vbattpc > 40)
            lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_2);
        else if(vbattpc > 20)
            lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_1);
        else
            lv_label_set_text_fmt(icBatt, LV_SYMBOL_BATTERY_EMPTY);
    }
}