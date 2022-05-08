# 06_menu_library
## Table of Contents
- [06_menu_library](#06_menu_library)
  - [Table of Contents](#table-of-contents)
  - [Disclaimer](#disclaimer)
  - [What I've learned](#what-ive-learned)
    - [References and pointers a quick reminder](#references-and-pointers-a-quick-reminder)
    - [The problem](#the-problem)
    - [The solution](#the-solution)
   
## Disclaimer
I'm not a c++ developer nor a arduino expert. I try -with my developer experience- to mimic some classic development technics in order to build simple things. If I say some bs, feel free to make a PR to correct me ! 
## What I've learned
### References and pointers a quick reminder
When you use "basic arduino" you work in a "global environment" 
```cpp
int led = 4;
OneClass instance;
void myFunction(){
    // do something
}
void setup(){
    ...
}
void loop(){
    ...
}
```
`instance`, `led`, `myFunction`, `setup` and `loop` are in the same global scope so you can access them easily. If I have to switch from a function to another I can reassign a function to work pointer  :
```cpp
void (*work)(); //that's how you create a pointer to a function with no argument

void caseOne(){
    Serial.println("Working on case one...");
    delay(3000);
    swithCaseToTwo(); 
}
void caseTwo(){
    Serial.println("Working on case two...");
    delay(3000);
}
void swithCaseToTwo(){
    // here we reasign the new work
    work = caseTwo;
}
void setup(){
  Serial.begin(9600);
  // we define our work to be caseOne
  work = caseOne;
}
void loop(){
    work();
}

```
With that in mind, I tried to pass a callback to a class : 

**Button.h**
```cpp
class Button
{
  public:
    Button(int pin);
    void onPress(void (*callback)()); //onPress member function needs a pointer to a function without arguments
  private:
    // _press is the variable where we store  the pointer
    void (*_press)();
};
```
**Button.cpp**
```cpp
#include "Button.h"
#include "Arduino.h"

Button::Button(int pin)
{
    pinMode(pin, INPUT);
}
void Button::press()
{
    if(_press){
        // triggers the stored callback function 
       _press();
    }
}
void Button::onPress(void (*callback)())
{
    _press = callback;
}
....
// later on, if the button is pressed we call
// press()
```
You can now have a basic program that listens to button events :

**arduino.ino**
```cpp
#include <Button.h>

Button button0(2); // we create a button on pin2

void setup() {
  Serial.begin(115200);
  button0.onRelease(&release0); //we pass a reference (&) to release0 global function 
}
void release0(){
  Serial.println(">RELEASE 0");
}
```
If you press the button `release0` function is triggered.
> All of those examples are from **02_button_library**

That was a short reminder.

### The problem

The problem now is if you work with multiple and imbricate classes you probably want to triggers member functions as callback (i do).
**It's way more complicated.**

If you have a `Menu` with `Button`s you want the menu to react to buttons. So you may pass `Menu` members functions to `Button` as we did before.

**Menu.cpp**
```cpp
Menu::Menu(){
    Button button0(2); //creates a button on pin2
    button.onRelease(&release); //pass the reference as above
}
Menu::release(){
    //yeah the button is released !
}
```
> ⚠️ This code doesn't work

This does not work because button onRelease function needs a `void(*callback)()`and we provide a `void Menu::release()`
. The compiler does not like it.


### The solution
The work around -I don't know if it's ok to do that in c++ world- is to use a static function to wrap the callback in.

**Menu.h**
```cpp
public:
  static void callback(void* pt2Object);
```
**Menu.cpp**
```cpp
// static callback function to be able to callback the member function
void Menu::callback(void* pt2Object)
{
    // explicitly cast to a pointer to Menu
    Menu* mySelf = (Menu*) pt2Object;
    mySelf->release0();
}
```
The `Menu::callback` takes a pointer to an arbitrary object, the object is casted to `Menu` then, the member function `release0` is called.

Now we need to modify `Button` class `onRelease` method to take the object pointer and the new created static callback method.

The new static callback method have a "long" signature `void (*Callback) (void* pt2Object);` we can use `typeDef` to assign it to a more conveniable type `Callback` 

**Button.h**
```cpp
typedef void (*Callback) (void* pt2Object);
public:
  Button(int pin);
  void update();  
  void onRelease(void* pt2Object, Callback callback);
```
Now, or `onRelease` method needs a pointer (to the menu instance) and a callback (the static function).

**Button.cpp**
```cpp
void Button::onPress(void* pt2Object,Callback callback)
{
  // we store the pointer and the callback
  _ptr = pt2Object;
  _press = callback;
}
void Button::press()
{
  if(_press){
    // if it exists we call the callback, with the pointer as parameter
    _press(_ptr);
  }
}
```

Now we can register callbacks to our Button.



