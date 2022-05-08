/*
  Button2.h - Library for button2 (librarie edition).
    06_menu_library 

*/

#ifndef Button2_h
#define Button2_h

enum button_states
{
  RELEASE0,
  RELEASE1,
  PRESS0,
  PRESS1
};
typedef void (*Callback)(void *pt2Object, button_states state);

class Button2
{
public:
  Button2(int pin);
  void update();
  void onRelease(void *pt2Object, Callback callback, button_states state);
  void onPress(void *pt2Object, Callback callback, button_states state);

private:
  void press();
  void release();
  void longpress();

  Callback _press;
  Callback _release;
  void *_ptrPress;
  void *_ptrRelease;

  button_states _stateRelease;
  button_states _statePress;

  int _pin;
  int previousState = 0;
  int currentState = 0;
  long lastDebounceTime = 0;
  long debounceDelay = 50;
  // longpress
  int longDuration = 750; // time to trigger a "long press" in ms
  int longState = 0;      // pushbutton current states (true if pressed for long)
  long longTime = 0;      // st
};

#endif