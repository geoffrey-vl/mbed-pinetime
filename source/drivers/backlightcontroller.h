#ifndef BACKLIGHTCONTROLLER_H
#define BACKLIGHTCONTROLLER_H

#include "DigitalOut.h"

enum class Brightness {
    OFF,
    LOW,
    MID,
    HIGH
};

class BacklightController
{
public:
    BacklightController();
    virtual ~BacklightController();

    void setBrightness(Brightness brightness);
    
private:
    mbed::DigitalOut mSlBrightnessLow;
    mbed::DigitalOut mSlBrightnessMid;
    mbed::DigitalOut mSlBrightnessHigh;
};
#endif // BACKLIGHTCONTROLLER_H