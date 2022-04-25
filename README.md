# Swarm Robotics // Sonic Ecosystems - Digital Innovation Season

### Central Saint Martins - 04/05/2022 
_created by Marguerite Tricaud_

***


## Project description

Sonic Ecosystems is a workshop created for the Digital Innovation Season 2022 at Central Saint Martins, London, UK. 

This workshop explores the creative potential of self-organising systems, distributed intelligence and modular design in the context of audiovisual interactive art. 
During the workshop, participants are invited to program the behaviour of small electronic devices to create emergent sonic and light patterns.

## Hardware

The code available in this repository runs on a specific device provided to each participant. 
All devices are identical, and they each comprise :

- A Teensy 4.0 
- A Teensy Audio Shield 4.0 (with an SD card)
- A speaker
- A light sensor 
- An LED
- A 4.5V battery pack

## Repository contents

This repository contains
- The source code to be uploaded onto the Teensy
- Sample audio files

## Installation steps

#### Arduino

Download and install the latest version of the [Arduino IDE](https://www.arduino.cc/en/software). 

#### Teensyduino

Download and install the latest version of [Teensyduino](https://www.pjrc.com/teensy/td_download.html). 

#### Audacity

Download and install the latest version of [Audacity](https://www.fosshub.com/Audacity.html). 

## User guide

#### Code

A variety of behaviour functions are available within the main source code. These are available at the end of the code after the loop, in the **Behaviours** section. 
Note : these functions are not compiled into a library so that participants can have access to them and change them if necessary.

These following behaviours are available :

- **volumeTrack()** : The device automatically plays the default audio sample, and adjust the playback volume to the brightness level in the room : the brighter the light, the louder the sound.
- **volumeTrack_inverse()** : The device automatically plays the default audio sample, and adjust the playback volume to the brightness level in the room : the brighter the light, the quieter the sound.
- **onoff()** : The device plays the sound sample when the brightness level is above a certain value
- **onoff_inverse()** : The device plays the sound sample when the brightness level is below a certain value
- **ledTest()** : A test fade on and off of the LED

#### Uploading code to the device

Make sure the device is turned off, to avoid over-using the battery.

Once you've plugged in the device to your computer with the micro-usb programming cable, make sure the right board and port are selected.

- Tools > Board > Teensyduino > Teensy 4.0
- Tools > Ports > Teensy Ports > _COM11 Serial (Teensy 4.0)_ 

You should be able to upload the code to the device.

#### Audio files 

- Format 
  * WAV mono track 
  * Rate : 44100 Hz 
  * Format  : 16 bit unsigned 
   * File name : uppercase 8.3 format 

- SD card
