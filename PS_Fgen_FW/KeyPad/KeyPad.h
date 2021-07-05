/**
 * @file	KeyPad.h
 * @date	03.09.2020 21:45:28
 * @author	Markus Scheich
 * @brief	Containing a function to check which key of the KeyPad is pressed.
 * @see		https://circuitdigest.com/microcontroller-projects/keypad-interfacing-with-avr-atmega32
 */ 

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../Pins/Pins.h"
#include "../UI_Lib/Core/Keys.h"

/**
 * Check which key of the KeyPad is pressed.
 * This function first powers the row pins and reads the value at the column pins.
 * Then the column pins are powered and the row pins are read.
 * From the combination of these values the pressed key is calculated.
 * This function always returns only one key. Multiple keys not supported with this structure.
 * @return Pressed key.
 */
Keys_t KeyPad_GetKeys();

#endif /* KEYPAD_H_ */