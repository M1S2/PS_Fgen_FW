/*
 * KeyPad.h
 *
 * Created: 03.09.2020 21:45:28
 *  Author: V17
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../Pins/Pins.h"
#include "../UI_Lib/Keys.h"

Keys_t KeyPad_GetKeys();
uint8_t KeyPad_GetKeyNumInt(Keys_t key);

#endif /* KEYPAD_H_ */