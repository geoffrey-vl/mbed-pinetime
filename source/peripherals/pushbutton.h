#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "DigitalIn.h"
#include "DigitalOut.h"

class PushButton
{
public:
    PushButton(PinName buttonPin, PinName enablePin);
    virtual ~PushButton();

    bool isHigh();

private:
    mbed::DigitalIn button;
    mbed::DigitalOut btnEnable;
};

#endif // PUSHBUTTON_H