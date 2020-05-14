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
    BacklightController(PinName backlighLow, PinName backlighMid, PinName backlighHigh);
    virtual ~BacklightController();

    Brightness brightness() { return mBrightness; }
    void setBrightness(Brightness brightness);
    void cycleBrightness();
    
private:
    mbed::DigitalOut mSlBrightnessLow;
    mbed::DigitalOut mSlBrightnessMid;
    mbed::DigitalOut mSlBrightnessHigh;
    Brightness mBrightness;
};
#endif // BACKLIGHTCONTROLLER_H