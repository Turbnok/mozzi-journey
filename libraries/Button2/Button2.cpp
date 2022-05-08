/*
    Button2.cpp - Library for Button2
*/

#include "Button2.h"
#include "Arduino.h"

Button2::Button2(int pin)
{
    pinMode(pin, INPUT);
    _pin = pin;
}

void Button2::release()
{
    if (_release)
    {
        _press(_ptrRelease, _stateRelease);
    }
    longState = false;
}
void Button2::press()
{
    longTime = millis();
    if (_press)
    {
        _press(_ptrPress, _statePress);
    }
}
void Button2::longpress()
{
    longState = true;
}

void Button2::onRelease(void *pt2Object, Callback callback, button_states state)
{
    Serial.println("onRelease");
    _ptrRelease = pt2Object;
    _release = callback;
    _stateRelease = state;
}

void Button2::onPress(void *pt2Object, Callback callback, button_states state)
{
    _ptrPress = pt2Object;
    _press = callback;
    _statePress = state;
}
void Button2::update()
{
    long elapsed = millis();
    int reading = digitalRead(_pin);
    if (reading != previousState)
    {
        // reset the debouncing timer
        lastDebounceTime = elapsed;
    }
    if ((elapsed - lastDebounceTime) > debounceDelay)
    {
        if (reading != currentState)
        {
            currentState = reading;
            if (currentState == 1)
            {
                press();
            }
            else
            {
                release();
            }
        }
    }
    previousState = reading;
    // long press
    // if the Button2 is pressed, the longState is false and the elapsed from the pressed time is greater than the longDuration
    if (currentState == HIGH && longState == false && (elapsed - longTime) > longDuration)
    {
        longpress();
    }
}