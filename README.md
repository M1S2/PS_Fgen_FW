# PS_Fgen_FW

[![GitHub Release Version](https://img.shields.io/github/v/release/M1S2/PS_Fgen_FW)](https://github.com/M1S2/PS_Fgen_FW/releases/latest)
[![GitHub License](https://img.shields.io/github/license/M1S2/PS_Fgen_FW)](LICENSE.md)
![Status](https://img.shields.io/badge/status-Development-blue)

Firmware for an PowerSupply and Functiongenerator build from an ATX power supply.

## Environment
The firmware is developed with the following environment:
|Parameter		| Value								|
|----------------------	| -------------------------------------------------------------	|
| Microcontroller	| ATmega1284P							|
| IDE			| Atmel Studio 7.0 (Windows 10 Environment)			|
| Compiler		| AVR GCC							|
| Programmer		| USBasp							|

## Installation
To download the firmware to the microcontroller either use a precompiled .hex file from a [release](https://github.com/M1S2/PS_Fgen_FW/releases/latest) or rebuild the project from the source files using the above IDE.
To download the .hex file you can use any programmer of your choice.
Make sure the fuse bits of the microcontroller are set as described in the [``main.cpp``](PS_Fgen_FW/main.cpp) file.

## User Interface
The user interface is made up of the following components:
- Graphical display: A 240x64 pixel graphical LCD is used to display the device status and current settings.
- Encoder: A rotary encoder can be used for value editing and menu navigation.
- KeyPad: A 16 button keypad can be used for value editing and menu navigation.
### GLCD handling
The u8glib library is used for GLCD handling:
[GitHub - olikraus/u8glib: Arduino Monochrom Graphics Library for LCDs and OLEDs](https://github.com/olikraus/u8glib)

### User Input Handling
<mark>RingBuffer...</mark>

### UI design
The user interface is based on a self developed UI library.
<mark>...</mark>

### UI Overview
To navigate between the main pages (Tabs), use the left and right arrow keys on the KeyPad. The Encoder is used to scroll between the sub pages and the individual controls on the sub pages. A control can be activated/deactivated by pressing the Encoder button. If a control is activated, the KeyPad and Encoder can be used to edit the controls value (depending on the control type).

<mark>Description of all UI pages...</mark>
- **SplashScreen:** <mark>...</mark>
	![Splash Screen](/Doc/Images/SplashScreen.jpg)
- **PS:** <mark>...</mark>
	- *PowerSupply:* <mark>...</mark>
		![Screen PS](/Doc/Images/Screen_PS.jpg)
	- *OVP:* <mark>...</mark>
		![Screen PS OVP](/Doc/Images/Screen_PS_OVP.jpg)
	- *OCP:* <mark>...</mark>
		![Screen PS OCP](/Doc/Images/Screen_PS_OCP.jpg)
	- *OPP:* <mark>...</mark>
		![Screen PS OPP](/Doc/Images/Screen_PS_OPP.jpg)
- **DDS:** <mark>...</mark>
	- *DDS1:* <mark>...</mark>
		![Screen DDS1](/Doc/Images/Screen_DDS1.jpg)
	- *DDS2:* <mark>...</mark>
		![Screen DDS2](/Doc/Images/Screen_DDS2.jpg)
- **Meas:** <mark>...</mark>
	- *DMM:*  <mark>...</mark>
		![Screen Meas DMM](/Doc/Images/Screen_Meas_DMM.jpg)
	- *ATX:* <mark>...</mark>
		![Screen Meas ATX](/Doc/Images/Screen_Meas_ATX.jpg)
- **Conf**: <mark>...</mark>
	- *Settings Device:* <mark>...</mark>
		![Screen Config Device](/Doc/Images/Screen_Conf_Device.jpg)
	-  *Display:* <mark>...</mark>
		![Screen Config Display](/Doc/Images/Screen_Conf_Display.jpg)
	- *Communication:* <mark>...</mark>
		![Screen Config Communication](/Doc/Images/Screen_Conf_Communication.jpg)
	- *PowerUp:* <mark>...</mark>
		![Screen Config PowerUp](/Doc/Images/Screen_Conf_PowerUp.jpg)
	- *Version:* <mark>...</mark>
		![Screen Config Version](/Doc/Images/Screen_Conf_Version.jpg)

## Configuration
The [Configuration.h](PS_Fgen_FW/Configuration.h) file contains some parameters that can be used to costumize the firmware. It has the following sections:
- **Version:** Version of the firmware. This should be adapted during release creation.
- **CPU Info:** Version of the controller board and type of the used microcontroller.
- **Timers:** Time delays used for key polling, screen drawing and some other device functions.
- **SCPI:** Use this section to configure buffers used by the SCPI parser or to set the IDN response elements. The SCPI subsystem can be completely removed from the build process (disabled), too.
- **Screen:** Screen settings including a option to disable the splash screen.
- **UserInputHandler:** The length of the user input queue can be defined here.
- **Channels:** The minimum and maximum values for the channel parameters are defined here. Also other parameters (e.g. PID values) are defined here.

## Device Class
The [``Device``](PS_Fgen_FW/Device.h) class bundles all class objects (like [``ScreenManagerClass``](PS_Fgen_FW/Screens/ScreenManager.h) or [``UserInputHandlerClass``](PS_Fgen_FW/UserInputHandler/UserInputHandler.h)), all channels and other device related settings together. It is also responsible for loading and saving all settings from/to the EEPROM. A global clock is also generated here used for PID regulators, KeyPad handling and some other components.

## Input / Output Channels
All input and output capabilites are derived from a base class [``Channel``](PS_Fgen_FW/Channels/Channel.h). At the moment there are the following channel types:
- ``POWER_SUPPLY_CHANNEL_TYPE``
The power supply channel has settings for the output voltage, current, enabled state. Also the over voltage protection (OVP), over current protection (OCP) and over power protection (OPP) can be configured. The actual voltage and current is also stored here.
The voltage is regulated to the required value using a software PID regulator.
<mark>PID regulator...</mark>

- ``DDS_CHANNEL_TYPE``
The DDS channel has settings for the output amplitude, offset, frequency, enabled state and signal form. It holds the table with all data points that are used by the DDS algorithm for signal creation.
The following waveforms are supported: SINE, RECTANGLE, TRIANGLE, SAWTOOTH.
<mark>DDS algorithm...</mark>
<mark>Frequency range...</mark>

- ``DMM_CHANNEL_TYPE``
The DMM channel simply holds the measured ADC voltage value of the corresponding DMM input channel.

## SCPI Interface
<mark>Commands, Parser, How to use, ...
The USB input can be used to communicate to the device via serial port.</mark>

## Hardware
The hardware for this power supply and function generator is developed in another repository:
[GitHub - M1S2/PS_Fgen_HW: Hardware for an Power Supply and Function Generator build from an ATX power supply](https://github.com/M1S2/PS_Fgen_HW)