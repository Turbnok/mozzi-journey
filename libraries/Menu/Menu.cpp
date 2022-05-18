/*
  Menu.cpp - Library for Menu
  with 2 buttons and 4 leds 
*/
#include "Menu.h"
#include "Arduino.h"
#include "FourLeds.h"
#define MAIN_MENUS 4 // we've got 4 main menus

Menu::Menu(uint8_t menu[4], FourLeds *fourleds)
{
  _menus = menu;
  _fourleds = fourleds;
  mainMenu = 0;
  subMenu = 0;
  
  _button0.onRelease(this, callback, RELEASE0);
  _button0.onPress(this, callback, PRESS0);
  _button1.onPress(this, callback, PRESS1);
}

// static wrapper-function to be able to callback the member function
void Menu::callback(void *pt2Object, button_states method)
{
  // explicitly cast to a pointer to Menu
  Menu *mySelf = (Menu *)pt2Object;
  switch (method)
  {
  case RELEASE0:
    mySelf->release0();
    break;
  case PRESS0:
    mySelf->press0();
    break;
  case PRESS1:
    mySelf->press1();
    break;
  default:
    break;
  }
}

void Menu::release0()
{
  Serial.println("Release 0");
  // on release wait half a second before setting the submenu value
  _fourleds->setValue(subMenu + 1, false, 500);
}
void Menu::press0()
{
  Serial.println("press 0");
  nextMainMenu();
}

void Menu::press1()
{
  Serial.println("press 1");
  nextSubMenu();
}

void Menu::nextSubMenu()
{
  subMenu++;

  if (subMenu == _menus[mainMenu])
  {
    subMenu = 0;
  }
  _fourleds->setValue(subMenu + 1);
}

void Menu::nextMainMenu()
{
  mainMenu++;
  subMenu = 0;
  if (mainMenu == MAIN_MENUS)
  {
    mainMenu = 0;
  }
  _fourleds->setValue(mainMenu + 1, true); // the second parameter clear the leds before setting the value
}

void Menu::update(long elapsed)
{
  _button0.update();
  _button1.update();
}