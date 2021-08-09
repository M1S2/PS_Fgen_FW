# ![Logo](logo.png) PS_Fgen_FW

[![GitHub Release Version](https://img.shields.io/github/v/release/M1S2/PS_Fgen_FW)](https://github.com/M1S2/PS_Fgen_FW/releases/latest)
[![GitHub License](https://img.shields.io/github/license/M1S2/PS_Fgen_FW)](LICENSE.md)
![Status](https://img.shields.io/badge/status-Development-blue)

Firmware for an PowerSupply and Functiongenerator build from an ATX power supply.

## Environment
The firmware is developed with the following environment:
|Name		| Value								|
|----------------------	| -------------------------------------------------------------	|
| Microcontroller	| ATmega1284P							|
| IDE			| Atmel Studio 7.0 (Windows 10 Environment)			|
| Compiler		| AVR GCC							|
| Programmer		| USBasp							|

## Installation
To download the firmware to the microcontroller either use a precompiled .hex file from a [release](https://github.com/M1S2/PS_Fgen_FW/releases/latest) or rebuild the project from the source files using the above IDE.
To download the .hex file you can use any programmer of your choice.
Make sure the fuse bits of the microcontroller are set as described in the [``main.cpp``](PS_Fgen_FW/main.cpp) file.

## Detailed Documentation
For the full doxygen generated documentation visit https://m1s2.github.io/PS_Fgen_FW

## User Interface
The user interface is made up of the following components:
- Graphical display: A 240x64 pixel graphical LCD is used to display the device status and current settings.
- Encoder: A rotary encoder can be used for value editing and menu navigation.
- KeyPad: A 16 button keypad can be used for value editing and menu navigation.
### GLCD handling
The u8glib library is used for GLCD handling:

[GitHub - olikraus/u8glib: Arduino Monochrom Graphics Library for LCDs and OLEDs](https://github.com/olikraus/u8glib)

### User Input Handling
All user inputs are directly enqueued into an RingBuffer when they occur. They are then later processed by the main loop. This approach is used to decouple the user input producers (ISRs) from the processing itself (which could take some time).
The following user inputs are detected:
- Keys: All pressed keys are enqueued as USERDATA_KEY types.
- Encoder: All encoder rotations (clockwise, counterclockwise) and the encoder button are also enqueued as USERDATA_KEY types. This is done to simplify the later processing by minimizing different data types.
- UART: Characters that are received via the serial communication interface are enqueued as USERDATA_USART types.

### UI design
The user interface is based on a self developed UI library.
There is a separate [Readme for the UI library.](/PS_Fgen_FW/UI_Lib/README.md)

### UI Overview
To navigate between the main pages (Tabs), use the left and right arrow keys on the KeyPad. The Encoder is used to scroll between the sub pages and the individual controls on the sub pages. A control can be activated/deactivated by pressing the Encoder button. If a control is activated, the KeyPad and Encoder can be used to edit the controls value (depending on the control type).

- **SplashScreen:** Welcome screen that shows the project name, the developer, the serial number of the device and the software version number.
	
	![Splash Screen](/Doc/Images/SplashScreen.jpg)
- **PS:** User interface part for the power supply channel of the device.
	- *PowerSupply:* This screen can be used to change the power supply voltage and current settings. Also the output can be enabled or disabled. Also the actual measurements (voltage, current, power) are shown.
		
		![Screen PS](/Doc/Images/Screen_PS.jpg)
	- *OVP:* Settings for the over voltage protection of the power supply channel (protection level in % of the configured output voltage, Enabled/Disabled state, protection delay after which the protection kicks in, button to clear an active protection)
		
		![Screen PS OVP](/Doc/Images/Screen_PS_OVP.jpg)
	- *OCP:* Settings for the over current protection of the power supply channel (protection level in % of the configured output current, Enabled/Disabled state, protection delay after which the protection kicks in, button to clear an active protection)
		
		![Screen PS OCP](/Doc/Images/Screen_PS_OCP.jpg)
	- *OPP:* Settings for the over power protection of the power supply channel (protection level in W, Enabled/Disabled state, protection delay after which the protection kicks in, button to clear an active protection)
		
		![Screen PS OPP](/Doc/Images/Screen_PS_OPP.jpg)
- **DDS:** User interface part for the direct digital synthesis channels of the device.
	- *DDS1:* This screen can be used to change the DDS parameters of channel 1 (frequency, waveform type, amplitude, offset, output enabled state).
		
		![Screen DDS1](/Doc/Images/Screen_DDS1.jpg)
	- *DDS2:* This screen can be used to change the DDS parameters of channel 2 (frequency, waveform type, amplitude, offset, output enabled state).
		
		![Screen DDS2](/Doc/Images/Screen_DDS2.jpg)
- **Meas:** User interface part for the measurement channels of the device.
	- *DMM:*  Show the measurements of both digital multimeter channels (DMM1 & DMM2) as value and as bar graph.
		
		![Screen Meas DMM](/Doc/Images/Screen_Meas_DMM.jpg)
	- *ATX:* Show the measurements of all ATX voltages as values and as bar graphs.
		
		![Screen Meas ATX](/Doc/Images/Screen_Meas_ATX.jpg)
- **Conf**: User interface part for different settings of the device.
	- *Settings Device:* This screen can be used to save all settings immediatelly (beside the automatic periodical saving), reset all settings and to start the calibration (guided calibration for all output and input channels).
		
		![Screen Config Device](/Doc/Images/Screen_Conf_Device.jpg)
	-  *Display:* Display settings can be used to invert the display color (background white, text non-white)
		
		![Screen Config Display](/Doc/Images/Screen_Conf_Display.jpg)
	- *Communication:* Settings for the serial communication interface (baud rate, Echo enabled/disabled)
		
		![Screen Config Communication](/Doc/Images/Screen_Conf_Communication.jpg)
	- *PowerUp:* Settings for the output states on power up. The ouputs can always be switched off or on on power up. Also the last output states can be restored on power up.
		
		![Screen Config PowerUp](/Doc/Images/Screen_Conf_PowerUp.jpg)
	- *Version:* Screen that shows the same version informations as the welcome screen (project name, developer, serial number of the device, software version number).
		
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
The voltage is regulated to the required value using a software PID regulator. An anti-windup scheme is also implemented with the PID integrator part to prevent integrator windup when the output is saturated.

	<mark>PID regulator...</mark>

- ``DDS_CHANNEL_TYPE``
The DDS channel has settings for the output amplitude, offset, frequency, enabled state and signal form. It holds the table with all data points that are used by the DDS algorithm for signal creation.
The following waveforms are supported: SINE, RECTANGLE, TRIANGLE, SAWTOOTH.

	<mark>DDS algorithm...</mark>
	<mark>Frequency range...</mark>

- ``DMM_CHANNEL_TYPE``
The DMM channel simply holds the measured ADC voltage value of the corresponding DMM input channel.

## SCPI Interface
The device supports SCPI (Standard Commands for Programmable Instruments) commands that can be used for remote control. All commands are received via the USB input (serial port).
There is a separate [Readme for the SCPI Part](/PS_Fgen_FW/SCPI/README_SCPI.md) that contains a detailed list of all supported commands.

## Hardware
The hardware for this power supply and function generator is developed in another repository:
[GitHub - M1S2/PS_Fgen_HW: Hardware for an Power Supply and Function Generator build from an ATX power supply](https://github.com/M1S2/PS_Fgen_HW)