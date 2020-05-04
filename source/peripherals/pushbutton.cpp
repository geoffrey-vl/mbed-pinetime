#include "pushbutton.h"

PushButton::PushButton(PinName buttonPin, PinName enablePin) :
    button(buttonPin, PullDown),
    btnEnable(enablePin, true)
{
}

PushButton::~PushButton()
{
}

bool PushButton::isHigh()
{
    return (button == 0 ? false : true);
}