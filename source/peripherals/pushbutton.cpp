#include "pushbutton.h"

#include "mbed_events.h"

#include "peripherals.h"

PushButton::PushButton(PinName buttonPin, PinName enablePin) :
    _button(buttonPin, PullDown),
    _btnEnable(enablePin, true),
    _touch_event_callback(nullptr)
{
    // Request the shared queue
    EventQueue *sharedEventQueue = mbed::mbed_event_queue();
    // attach event handler to interrupt pin
    _button.rise(sharedEventQueue->event(this, &PushButton::onRiseInterrupt)); 
    _button.fall(sharedEventQueue->event(this, &PushButton::onFallInterrupt)); 
}

PushButton::~PushButton()
{
}

bool PushButton::isHigh()
{
    return (_button == 0 ? false : true);
}

void PushButton::setCallBack(mbed::Callback<void(bool)> touch_event_callback)
{
    _touch_event_callback = touch_event_callback;
}

void PushButton::onRiseInterrupt()
{
    if (_touch_event_callback)
        _touch_event_callback(true);
}

void PushButton::onFallInterrupt()
{
    if (_touch_event_callback)
        _touch_event_callback(false);
}