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

#include <Arduino.h>

#include <tables/triangle2048_int8.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

#define AUDIO_MODE STANDARD_PLUS
#define CONTROL_RATE 64
#define SEQUENCER_STEPS 16
#define SEQUENCER_NB 3
#define ENVS_NB 4
#define MAIN_MENUS 4
constexpr float notes[12] = {261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883};

Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscK(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscS(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscB(TRIANGLE2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> oscM(SAW2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> mVibrato(SIN2048_DATA);
Oscil<SAW2048_NUM_CELLS, CONTROL_RATE> *oscs[2] = {&oscB, &oscM};

byte sequencer[SEQUENCER_NB][SEQUENCER_STEPS][4] = {
    {{1, 0, 0, 0}, {3, 0, 0, 0}, {2, 0, 0, 0}, {3, 0, 0, 0}, {3, 0, 0, 0}, {1, 0, 0, 0}, {3, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {3, 0, 0, 0}, {2, 0, 0, 0}, {3, 0, 0, 0}, {3, 0, 0, 0}, {1, 0, 0, 0}, {3, 0, 0, 0}, {2, 0, 0, 0}},
    {{8, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {8, 0, 0, 0}, {2, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 255, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
// octave base
// vibrato freq
// vibrato amount
// slide amount
// slide value
// envelop A
// envelop D
// envelop S
// envelop R
float synths[4][9] = {
    {2.3f, 0, 0, 1.0f, 0.0f, 20, 50, 25, 50},           // Kick
    {18.1f, 15.f, 128, 1.0f, 20, 20, 50, 30, 50},       // snare
    {1024.0f, 15.0f, 128, 1.0f, 0.0f, 20, 50, 50, 400}, // bass
    {256.0f, 8.f, 255, 1.01f, 20, 20, 100, 30, 3000},   // main
};

Pot pot0 = Pot(A0);
Pot pot1 = Pot(A1);

uint8_t menus[MAIN_MENUS] = {3, 5, 9, 2};
FourLeds fourleds(3, 11, 5, 6);
Menu menu(menus, &fourleds);

ADSR<CONTROL_RATE, AUDIO_RATE> envK;
ADSR<CONTROL_RATE, AUDIO_RATE> envS;
ADSR<CONTROL_RATE, AUDIO_RATE> envB;
ADSR<CONTROL_RATE, AUDIO_RATE> envM;
ADSR<CONTROL_RATE, AUDIO_RATE> *envelops[ENVS_NB] = {&envK, &envS, &envB, &envM};

unsigned long elapsed;
unsigned long last = 0;

int interval = 60000 / 120 / 4; // default BPM divided by 4 steps
uint8_t stepId = 0;

int kNoise = random(-127, 128);
int oldkValue = 0;

uint8_t kValue = 0;
uint8_t kick = 0;
uint8_t snare = 0;
uint8_t bass = 0;
uint8_t synth = 0;

uint8_t mix1 = 0;
uint8_t mix2 = 0;
uint8_t _mainVolume = 1.0f;
uint8_t output = 0;
void setup()
{
    Serial.begin(115200);
    Serial.println("-serial ok-");
    startMozzi(CONTROL_RATE);

    oscK.setFreq(2.3f);
    oscB.setFreq(0.2f);
    oscM.setFreq(1.0f);
    mVibrato.setFreq(2.f);
}
/** Updates synth frequencies for slides **/
void updateSynths()
{
    for (byte i = 2; i < 4; i++)
    {
        synths[i][4] = synths[i][4] * synths[i][3];
        oscs[i - 2]->setFreq(synths[i][4]);
    }
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
    updateSynths();
    updatePots();
    updateNotes();
}
void updatePots()
{
    if (pot0.hasChanged())
    {
        int val = pot0.getValue();
        if (menu.mainMenu == 0)
        {
            if (menu.subMenu == 0)
            {
                _mainVolume = val >> 2;
            }
        }
    }
    if (pot1.hasChanged())
    {
        int val = pot1.getValue();
        if (menu.mainMenu == 0)
        {
            if (menu.subMenu == 0)
            {
                interval = 60000 / ( val >> 2 ) / 4;
            }
        }
    }
}

void updateNotes()
{
    elapsed = millis();

    if ((elapsed - last) > interval)
    {
        stepId++;
        if (stepId == SEQUENCER_STEPS)
        {
            stepId = 0;
        }
        last = elapsed;
        for (byte i = 0; i < SEQUENCER_NB; i++)
        {
            byte note = sequencer[i][stepId][0];
            if (note != 0)
            {
                if (i == 0)
                {

                    if (note == 1)
                    {
                        oscK.setFreq(synths[i][0]);
                        envelops[i]->setADLevels(255, 255);
                        envelops[i]->setTimes(synths[i][5], synths[i][6], synths[i][7], synths[i][8]);
                        envelops[i]->noteOn();
                    }
                    else if (note == 2)
                    {
                        oscK.setFreq(synths[i + 1][0]);
                        envelops[i + 1]->setADLevels(255, 255);
                        envelops[i + 1]->setTimes(synths[i + 1][5], synths[i + 1][6], synths[i + 1][7], synths[i + 1][8]);
                        envelops[i + 1]->noteOn();
                    }
                    else if (note == 3)
                    {
                        oscK.setFreq(35.1f);
                        envelops[i]->setADLevels(50, 50);
                        envelops[i]->setTimes(5, 5, 5, 5);
                        envelops[i]->noteOn();
                    }
                }
                else
                {
                    synths[i + 1][4] = notes[note - 1] / synths[i + 1][0];
                    oscs[i - 1]->setFreq(synths[i + 1][4]);

                    envelops[i + 1]->setADLevels(255, 255);
                    envelops[i + 1]->setTimes(synths[i + 1][5], synths[i + 1][6], synths[i + 1][7], synths[i + 1][8]);
                    envelops[i + 1]->noteOn();
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
    if (kValue != oldkValue)
    {
        kNoise = rand(-127, 128);
    }
    oldkValue = kValue;

    kick = (((kNoise)*envK.next()) >> 8) + 128;
    snare = (((kNoise)*envS.next()) >> 8) + 128;
    bass = ((oscB.next() * envB.next()) >> 8) + 128;

    // Vibrato add so weird distortion
    // Q15n16 vibrato = (Q15n16) synths[1][2] * mVibrato.next();
    // synth = ((oscM.phMod(vibrato) * envM.next() ) >> 8) + 128;
    synth = ((oscM.next() * envM.next()) >> 8) + 128;

    if (kick < 128 && bass < 128)
    {
        mix1 = (kick * bass) >> 7;
    }
    else
    {
        mix1 = (kick + bass) * 2 - ((kick * bass) >> 7) - 256;
    }
    if (synth < 128 && snare < 128)
    {
        mix2 = (synth * snare) >> 7;
    }
    else
    {
        mix2 = (synth + snare) * 2 - ((synth * snare) >> 7) - 256;
    }
    if (mix1 < 128 && mix2 < 128)
    {
        output = (mix1 * mix2) >> 7;
    }
    else
    {
        output = (mix1 + mix2) * 2 - ((mix1 * mix2) >> 7) - 256;
    }
    return (output-128)*_mainVolume >> 8;
}
void loop()
{
    audioHook();
}
