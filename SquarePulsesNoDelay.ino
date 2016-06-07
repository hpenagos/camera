/*
Square pulses to drive PointGrey cameras in Rat1
 */

int ledPIN = 11;    
int inPIN = 7;

boolean triggered = false;
unsigned long pulseTimer = 0;
unsigned long pulseDur = 1;
unsigned long pulsePer = 32;

int buttonState;
int lastButtonState = HIGH;
boolean startRec = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

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
  
  unsigned long timekeeper = tElapsed(pulseTimer);
  
  if (triggered == false) {
    triggered = true;
    digitalWrite(ledPIN,HIGH);
    pulseTimer = millis();
  }
  else {
    if (startRec == true) {
      triggered = false;
      unsigned long newperiod;
      if (timekeeper <= 20) {
        newperiod = 20-timekeeper;
      }
      else {
        newperiod = pulsePer;
        }
      pulse(duration,newperiod);
      }
      
      else {
      if (timekeeper >= duration && timekeeper < period) {
      digitalWrite(ledPIN,LOW);
      }
      else if (timekeeper >= period) {
      triggered = false;
      }
    }
  }
}

unsigned long tElapsed(unsigned long timer) {
  unsigned long timediff;
  timediff = millis()-timer;
  return timediff;
}

void buttonStuff() {
  int reading = digitalRead(inPIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        startRec = true;
        pulse(pulseDur,4);
      }
    }
  }
  lastButtonState = reading;
}
