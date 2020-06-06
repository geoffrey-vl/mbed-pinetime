#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "I2C.h"
#include "SPI4Wire.h"

#include "backlightcontroller.h"
#include "batterymonitor.h"
#include "pushbutton.h"
#include "CST816S.h"
#include "ST7789LVGL.h"


namespace Peripherals {
    //busses
    extern mbed::I2C i2c;
    extern SPI4Wire spiDisplayInterface;

    //peripherals
    extern ST7789LVGL lcd;
    extern BacklightController backlight;
    extern BatteryMonitor batteryMonitor;
    extern PushButton pushButton;
    extern CST816S touchPad;
};

#endif // PERIPHERALS_H