/*
    Button.cpp - Library for button
*/

#include "Button.h"
#include "Arduino.h"

Button::Button(int pin)
{
    pinMode(pin, INPUT);
    _pin = pin;
}

void Button::release()
{
    if(_release){
        _release();
    }  
    longState = false;
}
void Button::press()
{
    longTime = millis();
    if(_press){
       _press();
    }
}
void Button::longpress()
{
    longState = true;
}

void Button::onRelease(void (*callback)())
{
    _release = callback;
}
void Button::onPress(void (*callback)())
{
    _press = callback;
}
void Button::update()
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
    // if the button is pressed, the longState is false and the elapsed from the pressed time is greater than the longDuration
    if (currentState == HIGH && longState == false && (elapsed - longTime) > longDuration)
    {
        longpress();
    }
}