/**
 * @file	Encoder.h
 * @date	07.09.2020 19:17:09
 * @author	Markus Scheich
 * @brief	Containing functions to handle the rotary encoder.
 * @see		https://www.leniwiec.org/en/2014/04/28/rotary-shaft-encoder-how-to-connect-it-and-handle-it-with-avr-atmega8-16-32-168-328/
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_

#include <avr/interrupt.h>
#include "../Pins/Pins.h"

/**
 * Initialize all registers for the pin interrupts used for encoder handling.
 */
void Encoder_Init();

/**
 * Check if the encoder button is pressed.
 * This checks if the ENC_PB pin is pulled low.
 * @return true if the button is pressed; otherwise false
 */
inline bool Encoder_IsButtonPressed()
{
	return BIT_IS_CLEARED(PIND, ENC_PB);	
}

#endif /* ENCODER_H_ */