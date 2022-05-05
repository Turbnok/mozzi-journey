/**
 * Catch longpress to emulate a "shift" behaviour
 * the point is just to not trigger release if  button is in shift mode
 * 
 * 2 buttons connected to pins 2 and 4
**/

#define NB_BTN 2 // we have 2 buttons

const uint8_t buttonPins[NB_BTN] = {2,4};       // pushbutton pins
// debouncing press / release
bool previousStates[NB_BTN] = {false,false};    // pushbuttons previous states (true if pressed)
bool currentStates[NB_BTN] = {false,false};     // pushbuttosn current states (true if pressed)
unsigned long lastDebounceTimes[NB_BTN] = {0,0};// the last time the output pin was toggled
unsigned long debounceDelay = 50;               // the debounce time; increase if the output flickers


// longpress
uint8_t longDuration = 750;                     // time to trigger a "long press" in ms
bool longStates[NB_BTN] = {false,false};        // pushbutton current states (true if pressed for long)
unsigned long longTimes[NB_BTN] = {0,0};        // stores the start press time

void setup() {
  Serial.begin(115200);
}

void releaseButton(uint8_t id){
  
  if(id==0){
    if(!longStates[id]){
      // don't trigger release if long press
      Serial.println("Button 0 is released");
    }
    //
  }else if(id==1){
    if(!longStates[id]){
      // don't trigger release if long press
      Serial.println("Button 1 is released");
    }
  }  
  longStates[id] = false;
}
void pressButton(uint8_t id){
  longTimes[id] = millis();
  if(id==0){
    Serial.println("Button 0 is pressed");
  }else if(id==1){
    Serial.println("Button 1 is pressed");
  }  
}
void longpressButton(uint8_t id){
  longStates[id] = true;
  if(id==0){
    Serial.println("Button 0 is longpressed");
  }else if(id==1){
    Serial.println("Button 1 is longpressed");
  }
}

void loop() {
  long elapsed = millis();
 
  for(uint8_t i=0;i<NB_BTN;i++){
    uint8_t reading = digitalRead(buttonPins[i]);  
  
    if (reading != previousStates[i]) {
      // reset the debouncing timer
      lastDebounceTimes[i] = elapsed;
    }
    if ((elapsed - lastDebounceTimes[i]) > debounceDelay) {
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

    // long press
    // if the button is pressed, the longState is false and the elapsed from the pressed time is greater than the longDuration
    if (currentStates[i] == HIGH &&  longStates[i] == false && ( elapsed - longTimes[i]) > longDuration)
    {
      longpressButton(i);
    }
  }
}
