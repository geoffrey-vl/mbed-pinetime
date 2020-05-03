#include "backlightcontroller.h"

BacklightController::BacklightController() :
    mSlBrightnessLow(LCD_BACKLIGHT_LOW, 1),
    mSlBrightnessMid(LCD_BACKLIGHT_MID, 1),
    mSlBrightnessHigh(LCD_BACKLIGHT_HIGH, 1),
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