#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define LOOPS         2
#define DELAY       300
#define NUM_FOLDERS   3
#define VOL_MAX      30
#define VOL_MIN       1
#define INC_VOL       3

// PINS 
#define BUTTON_NEXT      2
#define BUTTON_PAUSE     3
#define BUTTON_PREV      4
#define BUTTON_SELECT    5
#define BUTTON_VOL_UP   12
#define BUTTON_VOL_DOWN 13
#define PIN_BUSY         6

#define DEBUG_MODE false

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


boolean busy = false;
boolean paused = false;

uint8_t type; // DFPlayer reads
int     value; // DFPlayer reads
uint8_t folder; // current folder  
uint8_t song; // current song in folder
int     loopCount;
int     loops;
int     songs;
int     volume;

void setup() {  
    pinMode(BUTTON_PAUSE, INPUT_PULLUP);
    pinMode(BUTTON_NEXT,  INPUT_PULLUP);
    pinMode(BUTTON_PREV,  INPUT_PULLUP);
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_VOL_UP, INPUT_PULLUP);
    pinMode(BUTTON_VOL_DOWN, INPUT_PULLUP);
    pinMode(PIN_BUSY, INPUT);

    digitalWrite(BUTTON_NEXT,  HIGH);
    digitalWrite(BUTTON_PAUSE, HIGH);
    digitalWrite(BUTTON_PREV,  HIGH);
    digitalWrite(BUTTON_SELECT, HIGH);
    digitalWrite(BUTTON_VOL_UP, HIGH);
    digitalWrite(BUTTON_VOL_DOWN, HIGH);
    
    mySoftwareSerial.begin(9600);

    if (DEBUG_MODE) {
      Serial.begin(9600);
      delay(DELAY);
    }
    
    folder = 1;
    volume = 30;
    
    initDFPlayer();
    initSongLoops();
}

void loop() {
    checkVolume();
    checkLoop();
    checkPause();
    checkNext();
    checkPrev();
    checkFolder();
}

void initSongLoops(){
    loopCount = 1;
    song = 1;
    do {
      songs = myDFPlayer.readFileCountsInFolder((int) folder);
      delay(DELAY);
    } while (songs < 1);
    play();
}

boolean isBusy(){
  boolean current = !digitalRead(PIN_BUSY);
  if (current != busy){
    busy=current;
  }
  return busy;
}

boolean canPlay(){
   return loopCount <= LOOPS;
}

void checkVolume(){   
   if (digitalRead(BUTTON_VOL_UP) == LOW) {

        volume = volume + INC_VOL ;
        if (volume > VOL_MAX) {
            volume = VOL_MAX;         
        }
        debugInfo((String) "v+ " + volume);
        myDFPlayer.volume(volume); 
        delay(DELAY);
    }

    if (digitalRead(BUTTON_VOL_DOWN) == LOW) {
        volume = volume - INC_VOL ;
        if (volume < VOL_MIN) {
            volume = VOL_MIN;         
        }
        debugInfo((String) "v- " + volume);
        myDFPlayer.volume(volume); 
        delay(DELAY);
    }
  
  
}
void checkLoop(){       
      if ( canPlay() & !isBusy() & !paused) {
            debugInfo("auto next"); 
            song++;        
            if (song > songs) {
                song=1;
                loopCount++;
            }  
            if (!canPlay()) {
                 debugInfo("LOOPS reached"); 
            } else {
              play(); 
            }
     
      }

}
void checkFolder(){
    if (digitalRead(BUTTON_SELECT) == LOW) {
        folder = folder + 1;
        if (folder > NUM_FOLDERS) {
          folder = 1;
        }
        initSongLoops();
        delay(DELAY);
    }
    

}
void checkNext(){
  if (digitalRead(BUTTON_NEXT) == LOW && canPlay()) {
        song++;
        if (song > songs ) {
          song=1;
        }
        debugInfo("Next Song..");
        play();
        delay(DELAY);
    }
}

void checkPrev(){
    if (digitalRead(BUTTON_PREV) == LOW && canPlay()) {
        song--;
        if (song<1){
          song = songs;
        }
        debugInfo("Previous Song..");
        play();
        delay(DELAY);
    }
}

void checkPause(){
   if (digitalRead(BUTTON_PAUSE) == LOW && canPlay()) {
        if (!paused) {
            myDFPlayer.pause();
            paused = true;
            printCurrentSong();
            debugInfo("Paused..");
            
        } else {
            paused = false;
            myDFPlayer.start();
            printCurrentSong();
            debugInfo("Playing..");
        }
        delay(DELAY);
    }
}

void play(){
  printCurrentSong();  
  myDFPlayer.playLargeFolder(folder, song);
  paused=false; 
  delay(DELAY);
}

void printCurrentSong() {
   debugInfo( (String) "Folder:" + folder + "/" + NUM_FOLDERS +"; Song: " + song + "/" + songs + "; Loop " + loopCount + "/" + (String) LOOPS);
}


void initDFPlayer(){
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
        debugInfo("Unable to begin:");
        debugInfo("1.Please recheck the connection!");
        debugInfo("2.Please insert the SD card!");
        while (true);
    }
    
    debugInfo(F("DFPlayer Mini online."));

    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(volume); 
    myDFPlayer.EQ(DFPLAYER_EQ_POP);

    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

}

void debugInfo(String message){
  if (DEBUG_MODE){
    Serial.println(message);
  }
}
