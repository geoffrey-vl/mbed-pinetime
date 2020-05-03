#include "batterymonitor.h"

#define MAX_VBAT    3900 //maximum value of battery in millivolts
#define MIN_VBAT    3500 //minimum value of battery in millivolts

BatteryMonitor::BatteryMonitor(PinName batteryVoltagePin, PinName feedbackChargingPin) :
    adc(batteryVoltagePin),
    fbCharging(feedbackChargingPin)
{
}

BatteryMonitor::~BatteryMonitor()
{
}

uint16_t BatteryMonitor::batteryVoltage()
{
    /*
     * Calculating vBatt.
     * Note that the battery is connected to the ADC using a voltage devider,
     * so the measured value is only half if what it is in reality.
     * The ADC uses internally 3.3V as reference
     * The ADC has a 12-bit resolution (max value => 4095)
     * Formula: adcVoltage = (adcValue * 2) / (4095/3.3)
     * Note that mbed normalizes the 12-bit adc value to 16-bit unsigned short
     * Adjusted formula: adcVoltage = (adcValue * 2) / (65535/3.3)
     * Note: to avoid floating points we can instead use millivolts
     * Final formula: adcVoltage = (adcValue * 2000) / (65535/3.3)
     * 
     * In short:
     * vBatt = (adcValue * 2000) / 19859
     * 
     * Finally note that, due to mbed's normalization to full 16-bit range
     * we may run into overflow issues when we perform pultiplications on
     * it. For this reason we cast to 32-bit unsigned int.
     */ 
     
    uint32_t adcvalue16bit = (uint32_t) (adc.read_u16());
    return (uint16_t) ((adcvalue16bit*2000)/19859);
}

uint8_t BatteryMonitor::batteryPercentage()
{
    /**
     * Calculate percentage.
     * We'll use linear interpolation as algorithm.
     * However, the LiPo battery has a much more complex discharge curve.
     * See https://forum.pine64.org/showthread.php?tid=8147.
     * The range goes from MAX_VBAT until MIN_VBAT.
     * Anything lower than MIN_VBAT and we should shutdown the device.
     * Anything higher than MAX_VBAT is considered 100% charged.
     * To calculate, first measure millis, next limit value to fir our range.
     * Next, apply formula:
     * Percentage = ((millis - MIN_VBAT) / (MAX_VBAT - MIN_VBAT) * 100 )
     * We can avoid floating points in following manner:
     * (millis - MIN_VBAT) gives us a value within range 0 ... 400.
     * Dividing by 4 gives us the pc:
     * Percentage = (millis - MIN_VBAT) / 4
     */ 

    uint16_t millis = batteryVoltage();
    if(millis > MAX_VBAT)
        millis = MAX_VBAT;
    else if(millis < MIN_VBAT)
        millis = MIN_VBAT;

    return (uint8_t) ((millis - MIN_VBAT) / 4);
}

bool BatteryMonitor::isCharging()
{
    // input LOW = charging
    // input HIGH = running on battery
    return (!fbCharging ? true : false);
}