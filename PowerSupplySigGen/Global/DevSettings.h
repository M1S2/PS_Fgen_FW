/*
 * DevSettings.h
 *
 * Created: 15.09.2020 19:33:01
 *  Author: V17
 */ 


#ifndef DEVSETTINGS_H_
#define DEVSETTINGS_H_

//"Use the Save EEPROM fusebit so it wont get deleted every time you program the mcu."
//https://www.avrfreaks.net/forum/tut-c-using-eeprom-memory-avr-gcc
#include <avr/eeprom.h>
#include <stdbool.h>

void SaveSettings();
void LoadSettings();
void ResetDevice();

extern bool DevSettingsNeedSaving;

#define AVR_VCC		5.27

#endif /* DEVSETTINGS_H_ */