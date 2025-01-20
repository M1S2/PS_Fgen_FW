
# ![Logo](logo.png) PS_Fgen_FW

[![GitHub Release Version](https://img.shields.io/github/v/release/M1S2/PS_Fgen_FW)](https://github.com/M1S2/PS_Fgen_FW/releases/latest)
[![GitHub License](https://img.shields.io/github/license/M1S2/PS_Fgen_FW)](LICENSE.md)

Firmware for an PowerSupply and Functiongenerator build from an ATX power supply.

## Features
- **UI**: User interface with GLCD and user inputs via keypad and encoder.
- **SCPI:** Support for SCPI (Standard Commands for Programmable Instruments) commands that can be used for remote control.
- **Channels:** Support for different channel types including PowerSupply, DDS and DMM.

For more informations visit the detailed documentation: https://m1s2.github.io/PS_Fgen_FW

## Environment
The firmware is developed with the following environment:
|Name			| Value															|
|----------------------	| ---------------------------------------------------------------------------------------------------------------------	|
| Microcontroller	| ATmega1284P	[(Datasheet)](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42719-ATmega1284P_Datasheet.pdf)	|
| IDE			| Visual Studio Code with PlatformIO (Windows 11 Environment)	[Info](https://docs.platformio.org/en/latest/integration/ide/vscode.html)			|
| Programmer		| [USBasp - USB programmer for Atmel AVR controllers](https://www.fischl.de/usbasp/)					|

## Installation
To download the firmware to the microcontroller either use a precompiled .hex file from a [release](https://github.com/M1S2/PS_Fgen_FW/releases/latest) or rebuild the project from the source files using the above IDE.
To download the .hex file you can use any programmer of your choice.
Make sure the fuse bits of the microcontroller are set as described in the [``main.cpp``](PS_Fgen_FW/main.cpp) file.

## Detailed Documentation
For the full doxygen generated documentation visit https://m1s2.github.io/PS_Fgen_FW

## Release creation process
Do the following steps to create a release:
1. Make sure that everything is committed to the master branch and builds without errors.
2. Update the Changelog and commit it to the master.
3. Checkout a new release branch (adapt the version accordingly): `git checkout -b release/v0.0.0`
4. Update the VERSION in the version.h file: Replace the "[NO_RELEASE]" string by the version number (e.g. "v0.0.0")
5. Create a new commit (adapt the version accordingly): `git add .` and `git commit -m "Created release v0.0.0"`
6. Tag the release commit (adapt the version accordingly): `git tag v0.0.0`
7. Push everything: `git push --set-upstream origin release/v0.0.0` and `git push --tags`

## Hardware
The hardware for this power supply and function generator is developed in another repository:
[GitHub - M1S2/PS_Fgen_HW: Hardware for an Power Supply and Function Generator build from an ATX power supply](https://github.com/M1S2/PS_Fgen_HW)