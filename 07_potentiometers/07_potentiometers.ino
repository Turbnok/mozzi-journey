/**
Potentiometer sketch 
2 buttons 
2 pots
4 leds for 4 bars > 16 steps
**/
#include <MozziGuts.h>
#include <FourLeds.h>
#include <Menu.h>
#include <Pot.h>

#include <Oscil.h>
#include <tables/saw2048_int8.h>
#include <tables/sin2048_int8.h>
// UI
#define MAIN_MENUS 4 // we've got 4 main menus
// 4 menus with 3,5,9 and 2 submenus
int menus[MAIN_MENUS] = {3, 5, 9, 2};

Pot pot0 = Pot(A0);
Pot pot1 = Pot(A1);

FourLeds fourleds(3, 11, 5, 6);
Menu menu(menus, &fourleds);

// sounds
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> osc(SAW2048_DATA);
//Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc(SIN2048_DATA);
void setup()
{
    Serial.begin(115200);
    Serial.println("-serial ok-");
    startMozzi(CONTROL_RATE);

    osc.setFreq(440); // fréquence de la note
}

void update()
{
    long elapsed = millis();
    menu.update(elapsed);
    pot0.update();
    pot1.update();
    fourleds.update(elapsed);
}
void updateControl()
{
     update();
    int poti = pot0.getValue();
    if (pot0.hasChanged())
    {
        int pot = ((float) pot0.getValue() / 1024) * AUDIO_RATE/2;        
        osc.setFreq(pot);
    }
}
int updateAudio()
{
  return osc.next();
}
void loop()
{
   
    audioHook();
}
