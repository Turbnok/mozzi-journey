/*
    Pot.cpp - Library for Pot
    Potentiometer librarie
    I had a hasChanged method in order to trigger events only if you move pot
*/

#include "Pot.h"
#include "Arduino.h"
#include <MozziGuts.h>

Pot::Pot(int pin)
{
    pinMode(pin, INPUT);
    _pin = pin;
}

int Pot::getValue()
{
    return _value;
}
bool Pot::hasChanged()
{
    return _hasChanged;
}
void Pot::update()
{
    int currentValue = mozziAnalogRead(_pin); 
    if (currentValue > (_value + 2) || currentValue < (_value - 2))
    {
        _value = currentValue;
        _hasChanged = true;
    }else{
        _hasChanged = false;
    }   
}