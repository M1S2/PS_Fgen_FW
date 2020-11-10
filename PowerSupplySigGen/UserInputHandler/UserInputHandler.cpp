/*
 * UserInputHandler.cpp
 *
 * Created: 08.09.2020 19:31:42
 *  Author: V17
 */ 

#include "UserInputHandler.h"

#include "../USART/USART.h"
#include "../Screens/ScreenManager.h"

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
				Usart0TransmitStr("KEY");
				Usart0Transmit(KeyPad_GetKeyNumInt(data->Key));
				
				ScreenManager.KeyInput(data->Key);
				break;
			}
			case USERDATA_ENCODER: 
			{	
				if(data->EncDir == ENCCLOCKWISE) { Usart0TransmitStr("ENC_CW"); }
				else { Usart0TransmitStr("ENC_CCW"); }
				
				ScreenManager.EncoderInput(data->EncDir);
				break;
			}
			case USERDATA_ENCODER_BUTTON:
			{
				Usart0TransmitStr("ENC_PB");
				
				ScreenManager.EncoderPBInput();
				break;
			}
			case USERDATA_USART: 
			{
				Usart0TransmitStr("D");
				Usart0Transmit(data->UsartChr);
				break;
			}
			default: break;
		}
	}
}