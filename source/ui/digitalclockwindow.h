#ifndef DIGITALCLOCKWINDOW_H
#define DIGITALCLOCKWINDOW_H

#include "basewindow.h"

class DigitalClockWindow : public BaseWindow
{
public:
    DigitalClockWindow(LVGLDisplayDriver& driver);
    virtual ~DigitalClockWindow();

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

#endif // DIGITALCLOCKWINDOW_H