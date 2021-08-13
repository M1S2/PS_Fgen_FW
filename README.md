
# ![Logo](logo.png) PS_Fgen_FW

[![GitHub Release Version](https://img.shields.io/github/v/release/M1S2/PS_Fgen_FW)](https://github.com/M1S2/PS_Fgen_FW/releases/latest)
[![GitHub License](https://img.shields.io/github/license/M1S2/PS_Fgen_FW)](LICENSE.md)
![Status](https://img.shields.io/badge/status-Development-blue)

Firmware for an PowerSupply and Functiongenerator build from an ATX power supply.

## Features
- **UI**: User interface with GLCD and self developed UI library. User Inputs via keypad and encoder.
- **SCPI:** Support for SCPI (Standard Commands for Programmable Instruments) commands that can be used for remote control.
- **Channels:** Support for different channel types including PowerSupply, DDS and DMM.

For more informations visit the detailed documentation: https://m1s2.github.io/PS_Fgen_FW

## Environment
The firmware is developed with the following environment:
|Name			| Value															|
|----------------------	| ---------------------------------------------------------------------------------------------------------------------	|
| Microcontroller	| ATmega1284P	[(Datasheet)](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42719-ATmega1284P_Datasheet.pdf)	|
| IDE			| Atmel Studio 7.0 (Windows 10 Environment)	[Download](https://atmel-studio.software.informer.com/)			|
| Compiler		| [AVR GCC](https://gcc.gnu.org/wiki/avr-gcc), [More AVR GCC examples](https://rn-wissen.de/wiki/index.php/Avr-gcc)	|
| Programmer		| [USBasp - USB programmer for Atmel AVR controllers](https://www.fischl.de/usbasp/)					|

## Installation
To download the firmware to the microcontroller either use a precompiled .hex file from a [release](https://github.com/M1S2/PS_Fgen_FW/releases/latest) or rebuild the project from the source files using the above IDE.
To download the .hex file you can use any programmer of your choice.
Make sure the fuse bits of the microcontroller are set as described in the [``main.cpp``](PS_Fgen_FW/main.cpp) file.

## Detailed Documentation
For the full doxygen generated documentation visit https://m1s2.github.io/PS_Fgen_FW

## Hardware
The hardware for this power supply and function generator is developed in another repository:
[GitHub - M1S2/PS_Fgen_HW: Hardware for an Power Supply and Function Generator build from an ATX power supply](https://github.com/M1S2/PS_Fgen_HW)