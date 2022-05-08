/*
    Button.cpp - Library for button
*/

#include "FourLeds.h"
#include "Arduino.h"

FourLeds::FourLeds(int pin0, int pin1, int pin2, int pin3)
{
    _pins[0] = pin0;
    _pins[1] = pin1;
    _pins[2] = pin2;
    _pins[3] = pin3;
    
    for (int i = 0; i < 4; i++)
    {
      pinMode(_pins[i], OUTPUT);
    }
}

void FourLeds::setValue(int value, bool clear, int delay){
  _value = value;
  _time = millis();
  _delay = delay;
  if(clear){    
    for( int i = 0;i<4;i++ ){
      analogWrite(_pins[i],0);
    }
    return;
  }
  if(delay){
    return;
  }
  _time = 0;
  int base = value >> 2; //divide by 4
  int left = value % 4; // what's left when you divide by 4
  for( int i = 0;i<4;i++ ){
    int brightness = (base*64);    
    if(i<left){
        brightness += 64;
    } 
    analogWrite(_pins[i], max(0, brightness-1) );
  }
}
void FourLeds::update(long elapsed){
  if(_time!=0 && ((elapsed-_delay) - _time ) > 100){
    setValue(_value,false);
  }
}