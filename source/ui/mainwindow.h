#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basewindow.h"

class MainWindow : public BaseWindow
{
public:
    MainWindow(LVGLDisplayDriver& driver);
    virtual ~MainWindow();

    void draw() override;

private:
    lv_obj_t * icBatt;
    lv_obj_t * lblBatt;
    lv_obj_t * lblTime;
    lv_obj_t * lblDay;
    time_t currentTime;
    uint8_t vbattpc;
    bool isCharging;

    void setTxtDateTime();
    void setTxtBattPc();
};

#endif // MAINWINDOW_H