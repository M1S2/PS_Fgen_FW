/*
 * Encoder.cpp
 *
 * Created: 07.09.2020 19:17:17
 *  Author: V17
 */ 

#include "Encoder.h"
#include "../UserInputHandler/UserInputHandler.h"
#include "../Device.h"

void Encoder_Init()
{
	// see: https://www.leniwiec.org/en/2014/04/28/rotary-shaft-encoder-how-to-connect-it-and-handle-it-with-avr-atmega8-16-32-168-328/
	EIMSK |= (1 << INT0);		// enable INT0
	EICRA |= (1 << ISC01);		// INT0 - falling edge
}

bool Encoder_IsButtonPressed()
{
	return BIT_IS_CLEARED(PIND, ENC_PB);
}

//INT0 interrupt
ISR(INT0_vect)
{
	if(!BIT_IS_CLEARED(PIND, ENC_B))
	{
		Device.UserInputHandler.EnqueueKeyInput(KEYUP);
	}
	else
	{
		Device.UserInputHandler.EnqueueKeyInput(KEYDOWN);
	}
}
