/**
 * @file	Keys.h
 * @date	17.03.2021 18:52:50
 * @author	Markus Scheich
 * @brief	Containing definitions for all available keys.
 */ 

#ifndef KEYS_H_
#define KEYS_H_

/**
 * Available input keys.
 * Enumeration with all different keys.
 */
typedef enum Keys
{
	KEYNONE,		/**< No key */
	KEY0,			/**< Number 0 key */
	KEY1,			/**< Number 1 key */
	KEY2,			/**< Number 2 key */
	KEY3,			/**< Number 3 key */
	KEY4,			/**< Number 4 key */
	KEY5,			/**< Number 5 key */
	KEY6,			/**< Number 6 key */
	KEY7,			/**< Number 7 key */
	KEY8,			/**< Number 8 key */
	KEY9,			/**< Number 9 key */
	KEYKILO,		/**< Kilo (*1000) key */
	KEYMILLI,		/**< Milli (/1000) key */
	KEYX1,			/**< x1 (*1) key */
	KEYMINUS,		/**< Minus (negate) key */
	KEYLEFT,		/**< Left arrow key */
	KEYRIGHT,		/**< Right arrow key */
	KEYUP,			/**< Up arrow key */
	KEYDOWN,		/**< Down arrow key */
	KEYOK			/**< OK (Enter) key */
}Keys_t;

/**
 * Get a number from the key for all numeric keys (KEY0 .. KEY9).
 * If an invalid (non-numeric) key is given, this function returns 100.
 * @param key Key to convert to a number (only number keys KEY0 .. KEY9 allowed)
 * @return 0 for KEY0, 1 for KEY1, ..., 9 for KEY9; 100 for all other keys
 */
unsigned char Keys_GetKeyNumInt(Keys_t key);

#endif /* KEYS_H_ */