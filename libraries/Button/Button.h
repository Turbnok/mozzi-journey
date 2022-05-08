/*
  Button.h - Library for button.
    05_menu 
      add press
*/
#ifndef Button_h
#define Button_h

class Button
{
  public:
    Button(int pin);
    void update();  
    void onRelease(void (*releaseCallback)());
    void onPress(void (*pressCallback)());
  private:
    void press();
    void release();
    void longpress();

    void (*_press)();
    void (*_release)();
    
    int _pin;
    int previousState = 0;
    int currentState = 0;
    long lastDebounceTime = 0;
    long debounceDelay = 50;
    // longpress
    int longDuration = 750; // time to trigger a "long press" in ms
    int longState = 0;     // pushbutton current states (true if pressed for long)
    long longTime = 0; // st
};

#endif