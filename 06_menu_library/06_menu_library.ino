/**
 * Menu sketch 
 * iterates throught main menu with button 0
 * iterates throught sub menu with button 1
 * display menu during a couple of second
 * 2 buttons 
 * 4 leds for 4 bars > 16 steps
**/

#include <FourLeds.h>
#include <Menu.h>
#define MAIN_MENUS 4 // we've got 4 main menus
// 4 menus with 3,5,9 and 2 submenus
int menus[MAIN_MENUS] = {3, 5, 9, 2};

int mainMenu = 0;
int subMenu = 0;

FourLeds fourleds(3, 11, 5, 6);
Menu menu(menus, &fourleds);

void setup()
{
  Serial.begin(115200);
  Serial.println("-serial ok-");
}

void update()
{
  long elapsed = millis();
  menu.update(elapsed);
  fourleds.update(elapsed);
}
void loop()
{
  update();
}
