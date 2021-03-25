/*
 * Keys.h
 *
 * Created: 17.03.2021 18:52:50
 *  Author: V17
 */ 


#ifndef KEYS_H_
#define KEYS_H_

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

unsigned char Keys_GetKeyNumInt(Keys_t key);

#endif /* KEYS_H_ */