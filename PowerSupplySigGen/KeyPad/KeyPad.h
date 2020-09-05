/*
 * KeyPad.h
 *
 * Created: 03.09.2020 21:45:28
 *  Author: V17
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../Pins/Pins.h"
#include "../GLCD/u8g.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef enum Keys
{
	KEYNONE = 0,
	KEY0 = (1 << 0),
	KEY1 = (1 << 1),
	KEY2 = (1 << 2),
	KEY3 = (1 << 3),
	KEY4 = (1 << 4),
	KEY5 = (1 << 5),
	KEY6 = (1 << 6),
	KEY7 = (1 << 7),
	KEY8 = (1 << 8),
	KEY9 = (1 << 9),
	KEYKILO = (1 << 10),
	KEYMILLI = (1 << 11),
	KEYCOMMA = (1 << 12),
	KEYMINUS = (1 << 13),
	KEYLEFT = (1 << 14),
	KEYRIGHT = (1 << 15)
}Keys_t;


Keys_t KeyPad_GetKeys(u8g_t *u8g);
bool KeyPad_IsKeyPressed(Keys_t keys, Keys_t key);

#endif /* KEYPAD_H_ */