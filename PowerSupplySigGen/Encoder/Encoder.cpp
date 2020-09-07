/*
 * Encoder.cpp
 *
 * Created: 07.09.2020 19:17:17
 *  Author: V17
 */ 

#include "Encoder.h"

EncoderDirection_t EncoderDir;

void Encoder_Init()
{
	// see: https://www.leniwiec.org/en/2014/04/28/rotary-shaft-encoder-how-to-connect-it-and-handle-it-with-avr-atmega8-16-32-168-328/
	GICR |= (1<<INT0)|(1<<INT1);				// enable INT0 and INT1
	MCUCR |= (1<<ISC01)|(1<<ISC11)|(1<<ISC10);	// INT0 - falling edge, INT1 - raising edge
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
		EncoderDir = ENCCLOCKWISE;
	}
	else
	{
		EncoderDir = ENCCOUNTERCLOCKWISE;
	}
}

//INT1 interrupt
ISR(INT1_vect)
{
	if(!BIT_IS_CLEARED(PIND, ENC_A))
	{
		EncoderDir = ENCCLOCKWISE;
	}
	else
	{
		EncoderDir = ENCCOUNTERCLOCKWISE;
	}
}
