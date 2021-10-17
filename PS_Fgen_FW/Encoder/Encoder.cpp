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
	
	// Clear the INT0 occured flag by writing a logical one to it.
	// Normally the flag is cleared when the interrupt routine is executed.
	// But this doesn't seem to work if at least one DDS channel is enabled (when TIMER2_COMPA_vect ISR is enabled).
	// With this line the flag is cleared correct.
	EIFR |= (1<<INTF0);
}
