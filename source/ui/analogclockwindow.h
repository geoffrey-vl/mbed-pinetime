#ifndef ANALOGCLOCKWINDOW_H
#define ANALOGCLOCKWINDOW_H

#include "basewindow.h"

class AnalogClockWindow : public BaseWindow
{
public:
    AnalogClockWindow(LVGLDisplayDriver& driver);
    virtual ~AnalogClockWindow();

    void draw() override;

private:
    lv_obj_t * icBatt;
    lv_obj_t* analogClock;

    time_t currentTime;
    uint8_t vbattpc;
    bool isCharging;

    void setTxtBattPc();
    void setClock();
};

#endif // ANALOGCLOCKWINDOW_H