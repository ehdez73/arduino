
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define RX 8
#define TX 9
#define PIN_LED 5
#define PIN_SENSOR A4 // Audio level samples
#define PIN_BUSY 12
#define BUTTON 3

#define THRESHOLD_MIN 2030 // lower than this value switch off the LED
#define THRESHOLD_MAX 2050  // Higher than this value switch on the LED

#define IDLE_TIME 60000
#define DELAY 500

// Songs
#define INTRO 1
#define POWER_ME_OFF 2


#define VOLUME 25 //Set volume value (0~30).


boolean busy=false;
int songs = 7;
int sensorValue = 0;  // variable to store the value coming from the sensor
int bsy = 0;
int max = 0;
int min = 9999;
unsigned long lastPlayedMillis = -1;
int current=1;

SoftwareSerial mySoftwareSerial(RX,TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


void setup() {   

    pinMode(BUTTON, INPUT_PULLUP);
    digitalWrite(BUTTON, HIGH);
    digitalWrite(PIN_LED, LOW);
    
    pinMode(PIN_BUSY, INPUT);
    pinMode(PIN_LED, OUTPUT);

    // if analog input pin 1 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    randomSeed(analogRead(1));
    
    mySoftwareSerial.begin(9600);
    
    delay(DELAY);

    initDFPlayer(); 

    myDFPlayer.play(INTRO);

    digitalWrite(PIN_LED, HIGH);
     
}


void checkIdle(){

   if (lastPlayedMillis < 0) {
      lastPlayedMillis = millis();
   } else {
     unsigned long diff = millis() - lastPlayedMillis ;
     if (diff  > IDLE_TIME){
        // Too much time without interaction, play reminder to power off
        myDFPlayer.play(POWER_ME_OFF);
        lastPlayedMillis = millis();
     }
   }
}

void loop() {
    
    checkIdle();

   if (digitalRead(BUTTON) == LOW) {
     playNextSong();
   }

   lightLed();
   
}

void playNextSong(){    
     delay(DELAY);
     current = ((current + 1 ) % songs ) + 3;  // skip 2 songs
     myDFPlayer.play(current);
     lastPlayedMillis = millis();
}

void lightLed(){
 
    sensorValue = analogRead(PIN_SENSOR);
   
    if (sensorValue > THRESHOLD_MAX ) { 
      digitalWrite(PIN_LED, HIGH);
    }
   
    if (sensorValue < THRESHOLD_MIN ) { 
      digitalWrite(PIN_LED, LOW);
    }
}

void initDFPlayer(){

    myDFPlayer.begin(mySoftwareSerial);
    myDFPlayer.setTimeOut(500);

    //----Set volume----
    myDFPlayer.volume(VOLUME); 
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

}

