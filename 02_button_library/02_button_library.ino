/**
 * Now that the button works i put them in a library 
 * It's the same code as 01_buttons_longpress but in a class
 * 
 * In order to be updated in time I need to update each buttons on loop
 * 2 buttons connected to pins 2 and 4
**/

#include <Button.h>

Button button0(2);
Button button1(4);

void setup() {
  Serial.begin(115200);
  button0.onRelease(&release0);
  button1.onRelease(&release1);
}
void release0(){
  Serial.println(">RELEASE 0");
}
void release1(){
  Serial.println(">RELEASE 1");
}
void update(){
  button0.update();
  button1.update();
}
void loop() {
  update();
}
