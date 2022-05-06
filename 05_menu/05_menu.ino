/**
 * Menu sketch 
 * iterates throught main menu with button 0
 * iterates throught sub menu with button 1
 * display menu during a couple of second
 * 2 buttons 
 * 4 leds for 4 bars > 16 steps
**/

#include "Button.h"
#include <FourLeds.h>

#define MAIN_MENUS 4 // we've got 4 main menus
// 4 menus with 3,5,9 and 2 submenus
const int menus[MAIN_MENUS] = {3,5,9,2};

int mainMenu = 0;
int subMenu = 0;

Button button0(2);
Button button1(4);
FourLeds fourleds(3,11,5,6);

void setup() {
  Serial.begin(115200);
  Serial.println("-serial ok-");
  
  button0.onPress(&press0);
  button0.onRelease(&release0);
  button1.onPress(&press1);  
}
void release0(){
  // on release wait half a second before setting the submenu value
  fourleds.setValue(subMenu+1,false,500); 
}
void press0(){
  nextMainMenu();
}
void press1(){
  nextSubMenu();
}

void nextSubMenu(){
  subMenu++;
  if(subMenu==menus[mainMenu]){
    subMenu = 0;
  }
  fourleds.setValue(subMenu+1);
}
void nextMainMenu(){
  mainMenu++;
  subMenu = 0;
  if(mainMenu==MAIN_MENUS){
    mainMenu = 0;
  }
  fourleds.setValue(mainMenu+1,true); // the second parameter clear the leds before setting the value
}

void update(){
  long elpased = millis();
  button0.update();
  button1.update();
  fourleds.update(elpased);
}
void loop() {
  update();
}
