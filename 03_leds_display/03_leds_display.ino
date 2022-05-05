/**
 * Hello world for a minimal tracker
 * 4 leds for 4 bars > 16 steps
**/
#define NB_LEDS 4 // we have 4 leds

const uint8_t ledPins[4] = {3, 11, 5, 6}; // analog pins

#include <Button.h>

int count = 0;
Button button0(2);
Button button1(4);

void setup() {
  Serial.begin(115200);
  button0.onRelease(&release0);
  button1.onRelease(&release1);

  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }

  setLedsValue(1);
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
  setLedsValue(count);
}
void plusOne(){
  count++;
  if(count>16){
    count = 0;
  }
  setLedsValue(count);
}
void setLedsValue(int value){
  int base = value >> 2; //divide by 4
  int left = value % 4; // what's left when you divide by 4
  for( int i = 0;i<4;i++ ){
    int brightness = (base*64);    
    if(i<left){
        brightness += 64;
    } 
    analogWrite(ledPins[i], max(0, brightness-1) );
  }
}

void update(){
  button0.update();
  button1.update();
}
void loop() {
  update();
}
