/*
  Pot.h - Library for potentiometer.
  hasChanged() returns true or false if the user mooves the pot.
  it's usefull when you are cycling through menus to NOT update
  value if the user don't touch the knob.
*/

#ifndef Pot_h
#define Pot_h

class Pot
{
public:
  Pot(int pin);
  int getValue();
  bool hasChanged();
  void update();
private:
  int _value;
  int _hasChanged;
  int _pin;
};

#endif