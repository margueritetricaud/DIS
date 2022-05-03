/* 
 Digital Innovation Season - Swarm Programming // Sonic Ecosystems Workshop
 Created by Marguerite Tricaud
 Central Saint Martins - May 2022

 This is the source code for the devices provided to each participant. 
 This code runs on Teensy 4.0 combined with the Audio Shield Rev D.
 
 The parameters and behaviours that can be updated are in the following sections :
 - INPUT/OUTPUT PARAMETERS (in the intialisation section)
 - OUTPUT LOGIC (in the loop() section)
 
*/

//LIBRARIES
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//AUDIO SETUP FOR SD CARD AND TONE GENERATION

AudioPlaySdWav           playWav1;     //xy=263,553
AudioMixer4              mixer1;         //xy=422,705
AudioSynthWaveform       waveform1;     //xy=483,543
AudioOutputI2S           audioOutput;           //xy=633,710
AudioConnection          patchCord1(playWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playWav1, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, audioOutput, 0);
AudioConnection          patchCord4(mixer1, 0, audioOutput, 1);
AudioConnection          patchCord5(waveform1, 0, mixer1, 2);
AudioControlSGTL5000     sgtl5000_1;     //xy=593,849

int current_waveform=0;

float vol = 0.75;

// SD CARD ACCESS
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// LDR and LED pins
const int ldrPin = A3; //input pin for the LDR sensor 
const int ledPin = 9; //output PWM pin for the LED

//INPUT VARIABLES
int ldrVal=0; // variable to store the value of the LDR sensor
float ldrVal_filter=0; //filtered value of the LDR sensor to reduce noise
int ldrMin = 0; // minimum LDR input value
int ldrMax = 1024; // maximum LDR input value
int ledVal=0; // output LED brightness value - between 0 and 255
float count = 0; // counter for the led brightness fade
int dir = 1; // increment for the led brightness fade

//INPUT/OUTPUT PARAMETERS
char sample[] = "BIRD.WAV";  // replace this with the name of the WAV file you want to play from the SD card
int holdDelay = 100; // delay value for the highpass function (in milliseconds). The higher the value, the slower the device will be to turn the LED on when it receives a trigger
int freq = 440; // oscillator frequency

//-------------------------- SETUP ---------------------------//
void setup() {
  
  Serial.begin(9600); //serial communication between the teensy and the computer
  Serial.println("Device starting up");
  
   // ----- Audio Setup ---//
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
    }else{
       Serial.println("SD card found");
    }

    // waveform setup
      waveform1.frequency(freq);
      waveform1.amplitude(0);
      current_waveform = WAVEFORM_SINE;    
  
    // ----- Input/Output Setup -----//
    pinMode(ldrPin, INPUT);
    pinMode(ledPin, OUTPUT);
    ldrVal = analogRead(ldrPin);
    ldrVal_filter = ldrVal;

    Serial.println("Device ready");
}


// ------------------------------- MAIN LOOP --------------------------------- //
void loop() {

  /*------ INPUT READING -----*/
  
  ldrVal = analogRead(ldrPin);
  ldrVal_filter = ldrVal*0.3+ldrVal_filter*0.7;
  ldrVal_filter = map(ldrVal_filter, ldrMin, ldrMax, 0, 1024);
  //Serial.println(ldrVal_filter); //prints the filtered value of the LDR to the monitor 
  //Serial.println(vol);
   
  /*----- OUTPUT LOGIC -----*/

  ledTest_fade(); //On/Off fading of the LED
//  lowpass_led(600); // Only turns on the LED if the room brightness is below a certain threshold - between 0 and 1024
//  highpass_led(600); // Only turns on the LED if the room brightness is over a certain threshold - between 0 and 1024
//  bandpass_led(300,500); // Plays the audio and turns on the LED if the room brightness is within a certain range - between 0 and 1024
//  audioTest(); // Plays the audio sample
//  lowpass(600); // Plays the audio and turns on the LED if the room brightness is below a certain threshold - between 0 and 1024
//  highpass(600); // Plays the audio and turns on the LED if the room brightness is over a certain threshold - between 0 and 1024
//  bandpass(800,1000); // Plays the audio and turns on the LED if the room brightness is within a certain range - between 0 and 1024
//  volumeTrack(); // Automatically plays the audio, turns on the LED and adjust the volume of the track depending on the room brightness 
//  volumeTrack_inverse(); // Automatically plays the audio, turns on the LED and adjust the volume of the track depending on the room brightness - inversely   
//  waveform(); // Plays a sine tone and changes the frequency depending on the brightness level - the brighter the light, the higher the pitch.

}


// ------------------------- AUDIO FUNCTIONS ---------------------------------- //
void playFile(const char *filename)
{
  // Start playing the file if it's not already playing
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
  playFile(sample);  // filenames are always uppercase 8.3 format 
  vol = map(ldrVal_filter,0,1024,0,0.75);
  sgtl5000_1.volume(vol);
  ledVal = pow(255, vol);
  analogWrite(ledPin,ledVal) ;
}

void volumeTrack_inverse(){
  playFile(sample);  // filenames are always uppercase 8.3 format 
  vol = map(ldrVal_filter,0,1024,0.75,0);
  //Serial.println(vol);
  ledVal = pow(255, vol);
  analogWrite(ledPin,ledVal) ;
}

void lowpass(int ldrThresh){
  if(ldrVal >= ldrThresh){
    stopFile(sample);
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  < ldrThresh){
    playFile(sample);  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }
}

void lowpass_led(int ldrThresh){
  if(ldrVal >= ldrThresh){
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  < ldrThresh){
    digitalWrite(ledPin, HIGH);
  }
}

void highpass(int ldrThresh){
  if(ldrVal <= ldrThresh){
    delay(holdDelay);
    stopFile(sample);
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  > ldrThresh){
    delay(holdDelay);
    playFile(sample);  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }
}

void highpass_led(int ldrThresh){
  if(ldrVal <= ldrThresh){
    delay(holdDelay);
    digitalWrite(ledPin, LOW);
  }else if(ldrVal  > ldrThresh){
    delay(holdDelay);
    digitalWrite(ledPin, HIGH);
  }
}

void bandpass(int minRange, int maxRange){
  if(ldrVal >= minRange && ldrVal <= maxRange){
    playFile(sample);  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }else{
    stopFile(sample);
    digitalWrite(ledPin, LOW);
  }
}

void bandpass_led(int minRange, int maxRange){
  if(ldrVal >= minRange && ldrVal <= maxRange){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }
}

void bandstop(int minRange, int maxRange){
  if(ldrVal <= minRange && ldrVal >= maxRange){
    playFile(sample);  // filenames are always uppercase 8.3 format
    digitalWrite(ledPin, HIGH);
  }else{
    stopFile(sample);
    digitalWrite(ledPin, LOW);
  }
}

void bandstop_led(int minRange, int maxRange){
  if(ldrVal <= minRange && ldrVal >= maxRange){
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
  ledVal = pow(255, map(count,0,255,0,1));
  delay(10);
  analogWrite(ledPin, ledVal);
}

void audioTest(){
  playFile(sample);  // filenames are always uppercase 8.3 format
}

void waveform(){
  freq = map(ldrVal_filter,0,1024,20,700);
  waveform1.frequency(freq);
  waveform1.amplitude(0.5);
  waveform1.begin(current_waveform);
  ledVal = pow(255, map(ldrVal_filter,0,1024,0,1));
  analogWrite(ledPin,ledVal) ;
}
