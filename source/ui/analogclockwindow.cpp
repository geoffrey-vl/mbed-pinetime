#include "analogclockwindow.h"

#include "lv_disp.h"
#include "lv_label.h"

#include "lv_analogclock.h"
#include "lv_fonts_pinetime.h"

#include "peripherals.h"

#include <iostream>
#include <ctime>

AnalogClockWindow::AnalogClockWindow(LVGLDisplayDriver& driver) : 
    BaseWindow(),
    icBatt(nullptr),
    analogClock(nullptr),
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
    //update battery variables and draw
    isCharging = Peripherals::batteryMonitor.isCharging();
    vbattpc = Peripherals::batteryMonitor.batteryPercentage();
    setTxtBattPc();

    // create analog clock
    analogClock = lv_analogclock_create(lv_scr_act(), NULL);
    //Describe the color for the needles
    static lv_color_t needle_colors[3];
    needle_colors[LV_ANALOGCLOCK_NEEDLE_HOURS] = LV_COLOR_OLIVE;
    needle_colors[LV_ANALOGCLOCK_NEEDLE_MINUTES] = LV_COLOR_OLIVE;
    needle_colors[LV_ANALOGCLOCK_NEEDLE_SECONDS] = LV_COLOR_SILVER;
    lv_analogclock_set_needle_colors(analogClock, needle_colors);
    lv_obj_set_size(analogClock, 200, 200);
    lv_obj_align(analogClock, NULL, LV_ALIGN_CENTER, 0, 0);
    setClock();
}

AnalogClockWindow::~AnalogClockWindow()
{
    lv_obj_del(analogClock);
    lv_obj_del(icBatt);
}


void AnalogClockWindow::draw()
{
    //get new time, check charging feedback
    time_t now = time(0);
    bool isCurrentlyCharging = Peripherals::batteryMonitor.isCharging();

    // triggers to redraw parts of the UI:
    // - minute has passed
    // - charging state changed
    if( (now != currentTime) || (isCurrentlyCharging != isCharging) ) 
    { 
        //redraw time and date
        currentTime = now;
        setClock();

        // update battery monitor
        isCharging = isCurrentlyCharging;        
        vbattpc = Peripherals::batteryMonitor.batteryPercentage();
        setTxtBattPc();
    }

    BaseWindow::draw();
}

void AnalogClockWindow::setTxtBattPc()
{
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

void AnalogClockWindow::setClock()
{
    tm* localTm = localtime(&currentTime);
    lv_analogclock_time_t analogTime = { localTm->tm_hour, localTm->tm_min, localTm->tm_sec};
    lv_analogclock_set_value(analogClock, analogTime);
}