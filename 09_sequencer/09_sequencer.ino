/**
Sequencer sketch 
2 buttons 
2 pots
4 leds for 4 bars > 16 steps
**/
#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <Oscil.h>
#include <ADSR.h>
#include <Pot.h>
#include <Menu.h>
#include <FourLeds.h>



#include <tables/triangle2048_int8.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

#define AUDIO_MODE STANDARD_PLUS
#define CONTROL_RATE 32
#define SEQUENCER_STEPS 16
#define SEQUENCER_NB 3
#define ENVS_NB 4
#define MAIN_MENUS 4 // we've got 4 main menus
// 4 menus with 3,5,9 and 2 submenus
uint8_t menus[MAIN_MENUS] = {3, 5, 9, 2};
uint8_t mainMenu = 0;
uint8_t subMenu = 0;

FourLeds fourleds(3, 11, 5, 6);
Menu menu(menus, &fourleds);

Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscK(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscS(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscB(TRIANGLE2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscM(SAW2048_DATA);

byte sequencer[SEQUENCER_NB][SEQUENCER_STEPS][4] = {
    {
        {1,0,0,0},{0,0,0,0},{2,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{2,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{2,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{2,0,0,0},{2,0,0,0}
    },
    {
        {1,0,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
    },
    {
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
    }
};

Pot pot0 = Pot(A0);
Pot pot1 = Pot(A1);


ADSR<CONTROL_RATE, AUDIO_RATE> envK;
ADSR<CONTROL_RATE, AUDIO_RATE> envS;
ADSR<CONTROL_RATE, AUDIO_RATE> envB;
ADSR<CONTROL_RATE, AUDIO_RATE> envM;

ADSR<CONTROL_RATE, AUDIO_RATE> *envelops[ENVS_NB] = {&envK,&envS,&envB,&envM};

unsigned long elapsed;
unsigned long last = 0;

int interval = 60000 / 80 / 4; // default BPM divided by 4 steps
uint8_t stepId = 0;

int kNoise = random(-127,128);
int oldkValue = 0;

int _pot0 = 150;
int _pot1 = 0;

static uint8_t kValue = 0;
static uint8_t kick = 0;
static uint8_t snare = 0;
static uint8_t bass = 0;
static uint8_t synth = 0;

static int mix1 = 0;
static int mix2 = 0;
void setup()
{
    Serial.begin(115200);
    Serial.println("-serial ok-");
    startMozzi(CONTROL_RATE); 
    oscK.setFreq(1.3f);
    oscB.setFreq(0.2f);   
    oscM.setFreq(2.0f); 
}

void update()
{
    long elapsed = millis();
    pot0.update();
    pot1.update();
}
void updateControl()
{
    update();    
    
    elapsed = millis();  
    if ((elapsed - last) > interval)
    {
        stepId++;
        if (stepId == SEQUENCER_STEPS)
        {
            stepId = 0;
        }
        last = elapsed;
        
        for (byte i = 0; i <= SEQUENCER_NB; i++)
        {
            int note = sequencer[i][stepId][0];
            if(note!=0){   
                if(i == 0){                
                    if(note==1){
                        oscK.setFreq(2.0f);
                        envelops[i]->setADLevels(255, 255);
                        envelops[i]->setTimes(0, 50, 0, 50);
                        envelops[i]->noteOn();                    
                    }else if(note==2){
                        oscK.setFreq(12.1f);
                        envelops[i+1]->setADLevels(255, 255);
                        envelops[i+1]->setTimes(0, 50, 0, 150);
                        envelops[i+1]->noteOn();
                    }
                }else if(i==1 || i==2){ 
                    envelops[i+1]->setADLevels(255, 255);
                    envelops[i+1]->setTimes(0, 200, 0, 100);
                    envelops[i+1]->noteOn();
                }
            }
        }
    }   
    envS.update();
    envK.update();
    envB.update();
    envM.update();
}

int updateAudio()
{    
    kValue = oscK.next();
    if(kValue != oldkValue) kNoise = rand(-127,128);
    oldkValue = kValue;

    kick = ((( kNoise ) * envK.next()) >> 8)+128; 
    snare = ((( kNoise ) * envS.next()) >> 8)+128;
    bass = ((oscB.next() * envB.next()) >> 8)+128;
    synth = ((oscM.next() * envM.next()) >> 8)+128;
    if(kick < 128 && bass < 128){
        mix1 = (kick*bass) >> 7 ;
    }else{
        mix1 = (kick+bass)*2 - ( (kick*bass) >> 7) - 256;
    }
    if(synth < 128 && snare < 128){
        mix2 = (synth*snare) >> 7 ;
    }else{
        mix2 = (synth+snare)*2 - ( (synth*snare) >> 7) - 256;
    }
    if(mix1 < 128 && mix2 < 128){
        return (mix1*mix2) >> 7 ;
    }else{
        return (mix1+mix2)*2 - ( (mix1*mix2) >> 7) - 256;
    }
   
}
void loop()
{
    audioHook();
}
