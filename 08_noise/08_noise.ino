/**
Noise sketch 
2 buttons 
2 pots
4 leds for 4 bars > 16 steps
**/
#include <MozziGuts.h>
#include <Oscil.h>
#include <Pot.h>
#include <ADSR.h>
#include <tables/triangle2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#define CONTROL_RATE 64

Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> osc0(SQUARE_NO_ALIAS_2048_DATA);

ADSR<CONTROL_RATE, AUDIO_RATE> envelope; // notre enveloppe
Pot pot0 = Pot(A0);
Pot pot1 = Pot(A1);

unsigned long elapsed;
unsigned long last = 0;
int interval = 60000 / 35;

int step = 0;
int maxstep = 0;
int noise = 0;
int oldvalue = 0;
void setup()
{
    Serial.begin(115200);
    Serial.println("-serial ok-");
    startMozzi(CONTROL_RATE); 
    osc0.setFreq(0.1f);   
   
}
int poti = 0;
int _pot0 = 150;
int _pot1 = 0;
float slide = 1.0f;
int pos = 0;

void update()
{
    long elapsed = millis();
    pot0.update();
    pot1.update();
}
void updateControl()
{
    update();
    poti = pot0.getValue();
    if (pot0.hasChanged())
    {
        _pot0 = pot0.getValue()/4;        
        maxstep = _pot0;
      
    }
    if (pot1.hasChanged())
    {
        _pot1 = pot1.getValue();///1024;   
        float p = _pot1/15.0f;
    }
    // tempo
    elapsed = millis();  
    if ((elapsed - last) > interval)
    {
        slide = 1.0f;
        last = elapsed;
        envelope.setADLevels(255, 255);
        // réglage des durées de l'enveloppe en millisecondes.
        envelope.setTimes(0, 100, 0, 100);
        envelope.noteOn(); // on joue la note

    }else{
        //slide *= 1.05f;
    }
    float freq = (float) ( _pot1/35.0f);
    osc0.setFreq( freq * slide );
    envelope.update(); // mise à jour de l'enveloppe
}

int updateAudio()
{
    int value = osc0.next();
    if(value == 127 && value != oldvalue){
        noise = random(-244,243);      
    }
    oldvalue = value;
    return noise * envelope.next() >> 8;
}
void loop()
{
    audioHook();
}
