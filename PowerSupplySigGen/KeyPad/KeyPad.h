/*
 * KeyPad.h
 *
 * Created: 03.09.2020 21:45:28
 *  Author: V17
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../Pins/Pins.h"
#include <stdbool.h>
//#include <string.h>
//#include <stdlib.h>

typedef enum Keys
{
	KEYNONE,
	KEY0,
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEYKILO,
	KEYMILLI,
	KEYCOMMA,
	KEYMINUS,
	KEYLEFT,
	KEYRIGHT,
	KEYUP,
	KEYDOWN,
	KEYOK
}Keys_t;


Keys_t KeyPad_GetKeys();
uint8_t KeyPad_GetKeyNumInt(Keys_t key);

#endif /* KEYPAD_H_ */