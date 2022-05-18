/*
*  Menu.h - Library for Menu with Four Leds.
*/
#ifndef Menu_h
#define Menu_h

#include "FourLeds.h"
#include "Button2.h"

class Menu
{

public:
  Menu(uint8_t menu[4], FourLeds *fourleds);
  void update(long time);
  static void callback(void *pt2Object, button_states state);
  uint8_t subMenu = 0;
  uint8_t mainMenu = 0;
private:
  uint8_t *_menus;


  Button2 _button0 = Button2(2);
  Button2 _button1 = Button2(4);

  FourLeds *_fourleds;

  void press0();
  void press1();
  void release0();
  void nextMainMenu();
  void nextSubMenu();
};

#endif