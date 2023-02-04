
#define BUTTON 3
#define LED_RED 7
#define LED_GREEN 5
#define LED_YELLOW 6
#define DELAY 200


int currentMode = 4;
int totalModes = 5;


void setup() {   
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}


void nextMode() {
   currentMode = ((currentMode + 1 ) % totalModes );
   delay(500);
}

void walk() {
     int wait=200;
     ryg( HIGH, LOW, LOW);
     delay(wait);
     ryg( LOW, HIGH, LOW);
     delay(wait);
     ryg( LOW, LOW, HIGH);
     delay(wait);
     ryg( LOW, HIGH, LOW);
     delay(wait);
    
}

void progressive() {
     int wait=200;
     ryg( HIGH, LOW, LOW);
     delay(wait);
     ryg( HIGH, HIGH, LOW);
     delay(wait);
     ryg( HIGH, HIGH, HIGH);
     delay(wait);
     ryg( LOW, LOW, LOW);
     delay(wait); 
}

void upAndDown() {
     int wait=200;
     ryg( LOW, LOW, HIGH);
     delay(wait);
     ryg( LOW, HIGH, HIGH);
     delay(wait);
     ryg( HIGH, HIGH, HIGH);
     delay(wait*2);
     ryg( LOW, HIGH, HIGH);
     delay(wait);
      ryg( LOW, LOW, HIGH);
     delay(wait);
     ryg( LOW, LOW, LOW);
     delay(wait*2); 
}


void blinkAll() {
     int wait=200;
     ryg( HIGH, HIGH, HIGH);
     delay(wait);
     ryg( LOW, LOW, LOW);
     delay(wait); 
}

void blinkRandom() {
     int wait=200;
     ryg( randomStatus(), randomStatus(), randomStatus());
     delay(wait);
     ryg( LOW, LOW, LOW);
     delay(wait); 
}

int randomStatus() {
     if (random(1,10) > 5) {
       return HIGH;
     } else {
       return LOW;
     }
}


void ryg( int red, int yellow, int green) {
     digitalWrite(LED_GREEN, green);
     digitalWrite(LED_RED, red );
     digitalWrite(LED_YELLOW, yellow);
}


void runCurrentMode() {
  switch(currentMode) {
    case 0 : progressive(); break;
    case 1 : walk(); break;
    case 2 : blinkAll(); break;
    case 3 : blinkRandom(); break;
    case 4 : upAndDown(); break;

  }
}

void loop() {
   if (digitalRead(BUTTON) == LOW) {
     nextMode();
   }
   runCurrentMode();
}
