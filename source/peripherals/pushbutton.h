#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "InterruptIn.h"
#include "DigitalOut.h"
#include "platform/Callback.h"

class PushButton
{
public:
    PushButton(PinName buttonPin, PinName enablePin);
    virtual ~PushButton();

    bool isHigh();
    void setCallBack(mbed::Callback<void(bool)> touch_event_callback);

private:
    mbed::InterruptIn _button;
    mbed::DigitalOut _btnEnable;
    mbed::Callback<void(bool)> _touch_event_callback;

    void onRiseInterrupt();
    void onFallInterrupt();
};

#endif // PUSHBUTTON_H