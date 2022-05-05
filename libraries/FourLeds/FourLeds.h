/*
  FourLeds.h - Library for Four Leds.
*/
#ifndef FourLeds_h
#define FourLeds_h

class FourLeds
{
public:
  FourLeds(int pin0, int pin, int pin2, int pin3);
  void setValue(int value);
private:
  int _pins[4];
};

#endif