#include "backlightcontroller.h"

BacklightController::BacklightController(PinName backlighLow, PinName backlighMid, PinName backlighHigh) :
    mSlBrightnessLow(backlighLow, 1),
    mSlBrightnessMid(backlighMid, 1),
    mSlBrightnessHigh(backlighHigh, 1),
    mBrightness(Brightness::OFF)
{
    //pins are active low!
}

BacklightController::~BacklightController()
{ 
}

void BacklightController::setBrightness(Brightness brightness)
{
    mBrightness = brightness;
    switch(brightness)
    {
        case Brightness::HIGH:
            mSlBrightnessLow = 1;
            mSlBrightnessMid = 1;
            mSlBrightnessHigh = 0;
            break;
        case Brightness::MID:
            mSlBrightnessLow = 1;
            mSlBrightnessMid = 0;
            mSlBrightnessHigh = 1;
            break;
        case Brightness::LOW:
            mSlBrightnessLow = 0;
            mSlBrightnessMid = 1;
            mSlBrightnessHigh = 1;
            break;
        case Brightness::OFF:
        default:
            mSlBrightnessLow = 1;
            mSlBrightnessMid = 1;
            mSlBrightnessHigh = 1;
            break;
    }
}

void BacklightController::cycleBrightness()
{
    switch(mBrightness)
    {
        case Brightness::HIGH:
            setBrightness(Brightness::OFF);
            break;
        case Brightness::MID:
            setBrightness(Brightness::HIGH);
            break;
        case Brightness::LOW:
            setBrightness(Brightness::MID);
            break;
        case Brightness::OFF:
        default:
            setBrightness(Brightness::LOW);
            break;
    }
}