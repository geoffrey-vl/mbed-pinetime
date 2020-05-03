#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include "AnalogIn.h"
#include "DigitalIn.h"

class BatteryMonitor
{
public:
    BatteryMonitor(PinName batteryVoltagePin, PinName feedbackChargingPin);
    virtual ~BatteryMonitor();


    /**
     * @returns battery voltage in millivolts
     */ 
    uint16_t batteryVoltage();

    /**
     * @returns battery percentage
     */ 
    uint8_t batteryPercentage();

    /**
     * @returns true if battery is being charged
     */ 
    bool isCharging();

private:
    mbed::AnalogIn adc;
    mbed::DigitalIn fbCharging;
};

#endif // BATTERYMONITOR_H