/*
 * UserInputHandler.cpp
 *
 * Created: 08.09.2020 19:31:42
 *  Author: V17
 */ 

#include "UserInputHandler.h"

#include "../USART/USART.h"
#include "../Screens/ScreenManager.h"
#include "../libs/scpi_parser/scpi.h"

UserInputHandlerClass UserInputHandler;

void UserInputHandlerClass::EnqueueKeyInput(Keys_t userKeyInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputData keyInput(userKeyInput);
		_userInputRingBuffer.enqueue(&keyInput);
	}
}

void UserInputHandlerClass::EnqueueEncoderInput(EncoderDirection_t userEncoderInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputData encInput(userEncoderInput);
		_userInputRingBuffer.enqueue(&encInput);
	}
}

void UserInputHandlerClass::EnqueueEncoderButtonInput()
{
	if(!_userInputRingBuffer.full())
	{
		UserInputData encButtonInput(true);
		_userInputRingBuffer.enqueue(&encButtonInput);
	}
}

void UserInputHandlerClass::EnqueueUsartInput(uint8_t userDataInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputData dataInput(userDataInput);
		_userInputRingBuffer.enqueue(&dataInput);
	}	
}

void UserInputHandlerClass::ProcessInputs()
{
	while(!_userInputRingBuffer.empty())
	{
		UserInputData* data = _userInputRingBuffer.dequeue();		
		switch(data->DataType)
		{
			case USERDATA_KEY: 
			{
				ScreenManager.KeyInput(data->Key);
				break;
			}
			case USERDATA_ENCODER: 
			{		
				ScreenManager.EncoderInput(data->EncDir);
				break;
			}
			case USERDATA_ENCODER_BUTTON:
			{				
				ScreenManager.EncoderPBInput();
				break;
			}
			case USERDATA_USART: 
			{
				Usart0Transmit(data->UsartChr);		// Echo character
				scpi_handle_byte(data->UsartChr);
				break;
			}
			default: break;
		}
	}
}