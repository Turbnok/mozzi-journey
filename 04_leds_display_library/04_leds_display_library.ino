/**
 * Hello world for a minimal tracker
 * 4 leds for 4 bars > 16 steps
**/
#define NB_LEDS 4 // we have 4 leds

//const uint8_t ledPins[4] = {3, 11, 5, 6}; // analog pins

#include "FourLeds.h"
#include <Button.h>

int count = 0;
Button button0(2);
Button button1(4);
FourLeds fourleds(3,11,5,6);

void setup() {
  Serial.begin(115200);
  button0.onRelease(&release0);
  button1.onRelease(&release1);

  
  fourleds.setValue(0);
}
void release0(){
  Serial.println(">RELEASE 0");
  plusOne();
}
void release1(){
  Serial.println(">RELEASE 1");
  minusOne();
}

void minusOne(){
  count--;
  if(count<0){
    count = 16;
  }
  fourleds.setValue(count);
}
void plusOne(){
  count++;
  if(count>16){
    count = 0;
  }
  fourleds.setValue(count);
}

void update(){
  button0.update();
  button1.update();
}
void loop() {
  update();
}
