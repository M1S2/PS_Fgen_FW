/*
 * Keys.cpp
 *
 * Created: 25.03.2021 21:03:47
 *  Author: V17
 */ 

#include "Keys.h"

unsigned char Keys_GetKeyNumInt(Keys_t key)
{
	switch(key)
	{
		case KEY0: return 0;
		case KEY1: return 1;
		case KEY2: return 2;
		case KEY3: return 3;
		case KEY4: return 4;
		case KEY5: return 5;
		case KEY6: return 6;
		case KEY7: return 7;
		case KEY8: return 8;
		case KEY9: return 9;
		default: return 100;
	}
}