/*
*  FourLeds.h - Library for Four Leds.
*  04_leds_display_library 
*   initial class 
*/
#ifndef FourLeds_h
#define FourLeds_h

class FourLeds
{
public:
  FourLeds(int pin0, int pin, int pin2, int pin3);
  void update(long time);
  void setValue(int value, bool clear=false, int delay=0);
private:
  int _pins[4];
  int _value;
  int _delay;
  long _time;
};
#endif