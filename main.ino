#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//AUDIO VARIABLES
AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;
float vol = 0.7;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// LDR and LED
int ldrPin = A3;
int ledPin = 9;
int ldrVal=0;
float ldrVal_filter=0;
int ldrMin = 0; // minimum LDR input value
int ldrMax = 1024; // maximum LDR input value
int ledVal=0; // output LED brightness value - between 0 and 255
int ldrThresh = 600; // brightness threshold - to calibrate depending on environment and source of light
int ldrThresh_min = 400; // brightness threshold min for bandpass function 
int ldrThresh_max = 600; // brightness threshold max for bandpass function 
int count = 0; // counter for the led brightness
int dir = 1; // increment for the led brightness

//--------------------------SETUP---------------------------//
void setup() {
  
  Serial.begin(9600);
  
   // ----- audio setup ---//
      // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
      AudioMemory(8);
     sgtl5000_1.enable();
     sgtl5000_1.volume(vol);
     SPI.setMOSI(SDCARD_MOSI_PIN);
      SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
      // stop here, but print a message repetitively
      while (1) {
        Serial.println("Unable to access the SD card");
        delay(500);
      }
    }
  
    // ----- input/output setup -----//
    pinMode(ldrPin, INPUT);
    pinMode(ledPin, OUTPUT);

    ldrVal = analogRead(ldrPin);
    ldrVal_filter = ldrVal;
}


// --------------------------------LOOP--------------------------------- //
void loop() {

  /*----INPUT READING----*/
  
  ldrVal = analogRead(ldrPin);
  ldrVal_filter = ldrVal*0.3+ldrVal_filter*0.7;
  ldrVal_filter = map(ldrVal_filter, ldrMin, ldrMax, 0, 1024);
  Serial.println(ldrVal_filter);

  /*----OUTPUT LOGIC----*/
  
    highpass_led(); // Only turns on the LED if the room brightness is over a certain threshold
  //lowpass_led(); // Only turns on the LED if the room brightness is below a certain threshold
  //bandpass_led(); // plays the audio and turns on the LED if the room brightness is within a certain range
  
  //highpass(); // Plays the audio and turns on the LED if the room brightness is over a certain threshold
  //lowpass(); // Plays the audio and turns on the LED if the room brightness is below a certain threshold
  //bandpass(); // plays the audio and turns on the LED if the room brightness is within a certain range
  
  //volumeTrack(); // Automatically plays the audio, turns on the LED and adjust the volume of the track depending on the room brightness 
  //volumeTrack_inverse(); // Automatically plays the audio, turns on the LED and adjust the volume of the track depending on the room brightness - inversely 
  
  //ledTest_fade(); //On/Off fading of the LED

}


// ------------------------- AUDIO FUNCTIONS ---------------------------------- //
void playFile(const char *filename)
{
  // Start playing the file.  This sketch continues to run while the file plays.
  if(!playWav1.isPlaying()){
    Serial.print("Playing file: ");
    Serial.println(filename);
    playWav1.play(filename);
  }
  // A brief delay for the library read WAV info
  delay(25);
}

void stopFile(const char *filename)
{
  if(playWav1.isPlaying()){
   Serial.print("Stop playback file: ");
   Serial.println(filename);
    playWav1.stop();
  }
  // A brief delay for the library read WAV info
  delay(25);
}


// ------------------------- BEHAVIOURS ---------------------------------- //

void volumeTrack(){
  playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format 
  vol = ldrVal_filter/1024;
  //Serial.println(vol);
  sgtl5000_1.volume(vol);
  analogWrite(ledPin, 255*vol);
}

void volumeTrack_inverse(){
  playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format 
  vol = ldrVal_filter/1024;
  //Serial.println(vol);
  sgtl5000_1.volume(1-vol);
  analogWrite(ledPin, 255*(1-vol));
}

void lowpasss(){
  if(ldrVal >= ldrThresh){
    stopFile("BIRD2.WAV");
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  < ldrThresh){
    playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }
}

void lowpasss_led(){
  if(ldrVal >= ldrThresh){
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  < ldrThresh){
    digitalWrite(ledPin, HIGH);
  }
}

void highpass(){
  if(ldrVal <= ldrThresh){
    stopFile("BIRD2.WAV");
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  > ldrThresh){
    playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }
}

void highpass_led(){
  if(ldrVal <= ldrThresh){
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  > ldrThresh){
    delay(20);
    digitalWrite(ledPin, HIGH);
  }
}

void bandpass(){
  if(ldrVal >= ldrThresh_min && ldrVal <= ldrThresh_max){
    playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }else{
    stopFile("BIRD2.WAV");
    digitalWrite(ledPin, LOW);
  }
}

void bandpass_led(){
  if(ldrVal >= ldrThresh_min && ldrVal <= ldrThresh_max){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }
}

void ledTest_fade(){

  if(count >=255){
    dir = -1;
  }else if(count <=0){
    dir = +1;
  }

  count += dir;
  analogWrite(ledPin, count);
  delay(20);
  
}
