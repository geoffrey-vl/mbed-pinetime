#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "backlightcontroller.h"
#include "batterymonitor.h"
#include "pushbutton.h"

namespace Peripherals {
    
static BacklightController backlight;
static BatteryMonitor batteryMonitor(BATTERY_VOLTAGE, PIN_CHARGE_INDICATION_IN);
static PushButton pushButton(PIN_PUSH_BUTTON_IN, PIN_PUSH_BUTTON_OUT);

};

#endif // PERIPHERALS_H