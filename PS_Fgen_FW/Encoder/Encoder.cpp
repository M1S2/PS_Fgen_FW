/*
 * Encoder.cpp
 * Created: 07.09.2020 19:17:17
 * Author: Markus Scheich
 */ 

#include "Encoder.h"
#include "../UserInputHandler/UserInputHandler.h"
#include "../Device.h"

//INT0 interrupt
void isrEncoder( void )
{
	if(digitalRead(PIN_NUMBER_ENC_B) == HIGH)
	{
		Device.UserInputHandler.EnqueueKeyInput(KEYUP);
	}
	else
	{
		Device.UserInputHandler.EnqueueKeyInput(KEYDOWN);
	}
}

void Encoder_Init()
{
	// enable and attach INT0
	attachInterrupt(0, isrEncoder, FALLING);
}
