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
#include "Core/Keys.h"

/**
 * Check which key of the KeyPad is pressed.
 * This function first powers the row pins and reads the value at the column pins.
 * Then the column pins are powered and the row pins are read.
 * From the combination of these values the pressed key is calculated.
 * This function always returns only one key. Multiple keys not supported with this structure.
 * @return Pressed key.
 */
inline Keys_t KeyPad_GetKeys()
{
	DDRC = 0xF0;				// taking row pins as output and column pins as input
	PORTC = 0x0F;				// powering the row pins
	
	uint8_t keypressed = 0;		// integer for storing matrix value
	Keys_t keys = KEYNONE;

	if (PINC != 0b00001111)		// if any of column pins goes high execute the loop
	{
		keypressed = PINC;		// taking the column value into integer
		PORTC ^= 0b11111111;	// powering column pins of keypad
		DDRC ^= 0b11111111;		// making rows as inputs and columns as outputs
		for(int i=0; i<8; i++) { asm volatile("nop"); }	// wait until PORTC has new value
		keypressed |= PINC;		// taking row value and ORing it to column value
		
		// Always returns only one key. Multiple keys not supported with this structure.
		switch(keypressed)
		{
			case 0b11101110: keys = KEY7; break;		// row 1 column 1
			case 0b11101101: keys = KEY8; break;		// row 1 column 2
			case 0b11101011: keys = KEY9; break;		// row 1 column 3
			case 0b11100111: keys = KEYKILO; break;		// row 1 column 4
			case 0b11011110: keys = KEY4; break;		// row 2 column 1
			case 0b11011101: keys = KEY5; break;		// row 2 column 2
			case 0b11011011: keys = KEY6; break;		// row 2 column 3
			case 0b11010111: keys = KEYMILLI; break;	// row 2 column 4
			case 0b10111110: keys = KEY1; break;		// row 3 column 1
			case 0b10111101: keys = KEY2; break;		// row 3 column 2
			case 0b10111011: keys = KEY3; break;		// row 3 column 3
			case 0b10110111: keys = KEYMINUS; break;	// row 3 column 4
			case 0b01111110: keys = KEYLEFT; break;		// row 4 column 1
			case 0b01111101: keys = KEY0; break;		// row 4 column 2
			case 0b01111011: keys = KEYRIGHT; break;	// row 4 column 3
			case 0b01110111: keys = KEYX1; break;	// row 4 column 4
			default: break;
		}
		
		DDRC ^= 0b11111111;		// making rows as outputs and columns as inputs
		PORTC ^= 0b11111111;	// powering row pins of keypad
	}
	return keys;
}

#endif /* KEYPAD_H_ */