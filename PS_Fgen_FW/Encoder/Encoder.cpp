/*
 * Encoder.cpp
 * Created: 07.09.2020 19:17:17
 * Author: Markus Scheich
 */ 

#include "Encoder.h"
#include "../UserInputHandler/UserInputHandler.h"
#include "../Device.h"

void Encoder_Init()
{
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
