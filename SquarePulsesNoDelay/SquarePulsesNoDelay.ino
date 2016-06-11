/* Square pulses to drive PointGrey cameras in Rat1 */

int ledPIN = 11; 
int inPIN = 7;

boolean triggered = false;
unsigned long pulseTimer = 0;
unsigned long pulseDur = 2;
unsigned long pulsePer = 33;
int lastpulseState = LOW;

int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

boolean startRec = false;
unsigned long startRecTimer = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPIN,OUTPUT);
  pinMode(inPIN,INPUT);
}

void loop() {
  pulse(pulseDur,pulsePer);
  buttonStuff();
}

void pulse(unsigned long duration, unsigned long period) {
  int currState = digitalRead(ledPIN);
  int pulseState;
  unsigned long timekeeper = tElapsed(pulseTimer);
  
  if (triggered == false) {
    triggered = true;
    pulseState = HIGH;
    pulseTimer = millis();
  }
  
  else {
    
    if (startRec == true) {
      triggered = false;
      pulseState = LOW;
      pulsePer = 50;
    }
    else {
      pulseState = currState;
      if (timekeeper >= duration) {
        pulseState = LOW;
        if (timekeeper >= period) {
          triggered = false;
        }   
      }
    }
  }
  digitalWrite(ledPIN,pulseState);
  if (tElapsed(startRecTimer) > 200) {
    pulsePer = 33;
  }
}

unsigned long tElapsed(unsigned long timer) {
  unsigned long timediff;
  timediff = millis()-timer;
  return timediff;
}

void buttonStuff() {
  startRec = false;
  int reading = digitalRead(inPIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (tElapsed(lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        startRec = true;
        startRecTimer = millis();
        /*pulse(pulseDur,4);*/
        Serial.print("start");
      }
    }
  }
  lastButtonState = reading;
}
