/*
 * Encoder.cpp
 * Created: 07.09.2020 19:17:17
 * Author: Markus Scheich
 */ 

#include "Encoder.h"
#include "../UserInputHandler/UserInputHandler.h"
#include "../Device.h"

uint8_t encoderLastEncAState_g;		/**< Variable used to hold the last state of the ENC_A input */

void Encoder_Init()
{
	// Reads the initial state of ENC_A
	encoderLastEncAState_g = BIT_IS_SET(PIND, ENC_A);
}

bool Encoder_IsButtonPressed()
{
	return BIT_IS_CLEARED(PIND, ENC_PB);
}

Keys_t Encoder_GetDirection()
{
	// Reads the "current" state of the ENC_A
	uint8_t encA_state = BIT_IS_SET(PIND, ENC_A);

	// If the previous and the current state of ENC_A are different, that means a Pulse has occured
	if(encA_state != encoderLastEncAState_g)
	{
		// If the ENC_B state is different to the ENC_A state, that means the encoder is rotating clockwise
		if ((BIT_IS_SET(PIND, ENC_B)) != encA_state)
		{
			return KEYUP; 
		}
		else
		{
			return KEYDOWN;
		}

		// Updates the previous state of ENC_A with the current state
		encoderLastEncAState_g = encA_state;
	}
	return KEYNONE;
}
