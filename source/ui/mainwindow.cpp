#include "mainwindow.h"

#include "lv_disp.h"
#include "lv_theme.h"
#include "lv_theme_nemo.h"

#include "lv_label.h"
#include <iostream>
#include <ctime>

MainWindow::MainWindow(LVGLDisplayDriver& driver) : 
    BaseWindow(),
    icBatt(nullptr),
    lblTime(nullptr),
    lblDay(nullptr),
    currentTime(time(0))
{
    // initialize LVGL
    lvgl.init();
	lvgl.add_display_driver(driver);
	lvgl.set_default_display(driver);

    // set the default theme to monochromatic
	lv_theme_t* theme = lv_theme_nemo_init(0, NULL);
	lv_theme_set_current(theme);

    lvgl.start();

    // battery icon
    icBatt = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(icBatt, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);  // Align to the top
	lv_obj_set_hidden(icBatt, false);
    lv_label_set_text(icBatt, LV_SYMBOL_BATTERY_FULL);

    // time label
	lblTime = lv_label_create(lv_scr_act(), NULL);
    static lv_style_t lbl_style_roboto28;
    lv_style_copy(&lbl_style_roboto28, &lv_style_plain);
    lbl_style_roboto28.text.font = &lv_font_roboto_28;  //Set a diferent font
    lv_label_set_style(lblTime, LV_LABEL_STYLE_MAIN, &lbl_style_roboto28);
	lv_obj_align(lblTime, NULL, LV_ALIGN_CENTER, 0, 0);  //Align to the top
	lv_obj_set_hidden(lblTime, false);
    // set time
    setTxtTime();

    lblDay = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(lblDay, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);  //Align to the top
	lv_obj_set_hidden(lblDay, false);
    // set calendar date
    setTxtDate();
}

MainWindow::~MainWindow()
{
    lv_obj_del(lblDay);
    lv_obj_del(lblTime);
    lv_obj_del(icBatt);
}

void MainWindow::draw()
{
    time_t now = time(0);
    if(now > (currentTime + 60)) { //less accurate but faster than calculate expensive modulo every 10ms
        currentTime = now;
        if(currentTime % 86400 == 0) { 
            //one full day has passed
            setTxtDate();
        }
        setTxtTime();
    }
    BaseWindow::draw();
}

void MainWindow::setTxtTime()
{
    tm *localTm = localtime(&currentTime);
    lv_label_set_text_fmt(lblTime, "%02d:%02d", localTm->tm_hour, localTm->tm_min); 
}

void MainWindow::setTxtDate()
{
    tm *localTm = localtime(&currentTime);
    lv_label_set_text_fmt(lblDay, "%02d/%02d/%04d", (localTm->tm_mday+1), (localTm->tm_mon+1), localTm->tm_year); 
}