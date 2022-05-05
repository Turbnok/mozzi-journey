/**
 * Hello world for pushing buttons with debouncing
 * 2 buttons connected to pins 2 and 4
**/
#define NB_BTN 2 // we have 2 buttons

const uint8_t buttonPins[NB_BTN] = {2,4};       // arduino pushbutton pins
bool previousStates[NB_BTN] = {false,false};    // pushbuttons previous states (true if pressed)
bool currentStates[NB_BTN] = {false,false};     // pushbuttosn current states (true if pressed)
unsigned long lastDebounceTimes[NB_BTN] = {0,0};// the last time the output pin was toggled
unsigned long debounceDelay = 50;               // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(115200);
}

void releaseButton(uint8_t id){
  if(id==0){
    Serial.println("Button 0 is released");
  }else if(id==1){
    Serial.println("Button 1 is released");
  }  
}
void pressButton(uint8_t id){
  if(id==0){
    Serial.println("Button 0 is pressed");
  }else if(id==1){
    Serial.println("Button 1 is pressed");
  }  
}

void loop() {

  for(uint8_t i=0;i<NB_BTN;i++){
    uint8_t reading = digitalRead(buttonPins[i]);  
    if (reading != previousStates[i]) {
      // reset the debouncing timer
      lastDebounceTimes[i] = millis();
    }
    if ((millis() - lastDebounceTimes[i]) > debounceDelay) {
      if (reading != currentStates[i]) {
        currentStates[i] = reading;   
        if (currentStates[i] == HIGH) {
            pressButton(i);            
        }else{
            releaseButton(i);      
        }
      }
    }
    previousStates[i] = reading;
  }
}
