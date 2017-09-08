# LoRa RN2483
An IoT project for demonstrating LoRa (an lpwan technology).
Developed for a Quark D2000, however it's very modular so not much needs changing for porting to other devices (i.e. ARM).
The main reason this is up here is for me to archive the semi-complete project and show off some C code I felt pretty good about at the time.

## Contents
1. [Overview](##Overview)
3. [Usage](##Usage)
4. [Porting](##Porting)
5. [Authors](##Authors)

## Overview
This is a project that was designed to demonstrate the usage of LoRa. It's written for a Quark D2000, however porting it over to another device (such as ARM) shouldn't be very difficult because of how I've tried to modularise it.
If anybody developing for the Quark actually used open-source libraries, i'd make an effort to finish the rn2483 library. Unfortunately, I've found that there is and will be very little need for it; so i've left it as is for now. I would make the effort to port it over to ARM, however there are already loads of really good ARM libraries you can find for nearly every LoRa device.
Currently the files in src/ are exported from an 'Intel System Studio for Microcontrollers' project, it should import into Eclipse or any fork. It doesn't rely on the project settings much for compiling or anything, so you should just be able to take the .h and .c files and add the to anything else.

If you want to use this project for a board that _isn't_ a Quark device, see [Porting](##Porting).

I've also left some useful reference documents in docs/.

## Usage
The project has been written in a very modular way. This means that if you just need to use the RN2483 part of it, then all you need to do is pull the rn2483.* files and make the changes specified in [Porting](##Porting).
The modules the project has been broken up into are:
* CFG.h, this is the most important file for any user. It contains all the different settings to change.
* RN2483, these files relate to the RN2483 device.
* Sensors, these files relate to r/w from the sensors (this includes pins, etc)
* Quark UART, this file relates to the UART interface on the quark.

## Porting
Not much needs to be changed to port this over to a non-quark device. The main change you'll need to make are the quark_uart.* files. The main function of this file is to setup the UART interface and set the stdout stream to that interface so that the other files can write to the RN2483 device (which communicates over UART) using printf().
Other than that most of what will need to be changed is in CFG.h/UART CONFIG and CFG.h/ADC CONFIG. These contain a lot of settings specific to the Quark D2000 (pin id's, etc). It's also worth checking the sensors.* files just to make sure they're not doing anything Quark specific - I made an effort to abstract them from quark specific fuctionality though.

## Authors
* [Alexander Collins](https://www.github.com/GeaRSiX)
