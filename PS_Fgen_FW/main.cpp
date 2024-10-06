/*
 * main.cpp
 * Created: 14.08.2020 19:23:21
 * Author: Markus Scheich
 */ 

/********************************************************
* ATMega 1284P
* avrdude -p m1284p -P usb -c usbasp 
* -U lfuse:w:0xFF:m (external Crystal 8-16 MHz)
* -U hfuse:w:0xD1:m (JTAG Interface Disable, Preserve EEPROM memory through the Chip Erase cycle)
* -U efuse:w:0xFC:m (Brown-out detection level at VCC=4.3 V)
********************************************************/

#include <Arduino.h>
#include "Device.h"

void setup()
{
	Device.Init();
}

void loop()
{
	Device.DeviceMainLoop();
}