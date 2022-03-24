
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//this is a test

AudioPlaySdWav           playWav1;
// Use one of these 3 output types: Digital I2S, Digital S/PDIF, or Analog DAC
AudioOutputI2S           audioOutput;
//AudioOutputSPDIF       audioOutput;
//AudioOutputAnalog      audioOutput;
//On Teensy LC, use this for the Teensy Audio Shield:
//AudioOutputI2Sslave    audioOutput;

AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14


int ldrPin = A16;
int ldrVal=0;

void setup() {
   Serial.begin(9600);
   
// Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
  pinMode(ldrPin, INPUT);

}

void loop() {
  ldrVal = analogRead(ldrPin)*0.5 + ldrVal*0.5;
  Serial.println(ldrVal);

  if(ldrVal < 500){
    playFile("BIRD2.WAV");  // filenames are always uppercase 8.3 format
  }else if(ldrVal  >=500){
    stopFile("BIRD2.WAV");
  }
}


void playFile(const char *filename)
{
  //Serial.print("Playing file: ");
 // Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  if(!playWav1.isPlaying()){
    playWav1.play(filename);
  }

  // A brief delay for the library read WAV info
  delay(25);

  // Simply wait for the file to finish playing.
  //while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  //}
}

void stopFile(const char *filename)
{
  //Serial.print("Playing file: ");
 // Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  if(playWav1.isPlaying()){
    playWav1.stop();
  }

  // A brief delay for the library read WAV info
  delay(25);

  // Simply wait for the file to finish playing.
  //while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  //}
}
