/*
 * UserInputHandler.cpp
 *
 * Created: 08.09.2020 19:31:42
 *  Author: V17
 */ 

#include "UserInputHandler.h"

#include "../USART/USART.h"

UserInputHandlerClass UserInputHandler;

void UserInputHandlerClass::EnqueueKeyInput(Keys_t userKeyInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputDataTypeKey keyInput(userKeyInput);
		_userInputRingBuffer.enqueue(&keyInput);
	}
}

void UserInputHandlerClass::EnqueueEncoderInput(EncoderDirection_t userEncoderInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputDataTypeEncoder encInput(userEncoderInput);
		_userInputRingBuffer.enqueue(&encInput);
	}
}

void UserInputHandlerClass::EnqueueEncoderButtonInput()
{
	if(!_userInputRingBuffer.full())
	{
		UserInputDataTypesBase encButtonInput(USERDATA_ENCODER_BUTTON);
		_userInputRingBuffer.enqueue(&encButtonInput);
	}
}

void UserInputHandlerClass::EnqueueUsartInput(uint8_t userDataInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputDataTypeUsart dataInput(userDataInput);
		_userInputRingBuffer.enqueue(&dataInput);
	}	
}

void UserInputHandlerClass::ProcessInputs()
{
	while(!_userInputRingBuffer.empty())
	{
		UserInputDataTypesBase* data = _userInputRingBuffer.dequeue();		
		switch(data->DataType)
		{
			case USERDATA_KEY: 
			{
				UserInputDataTypeKey* dataKey = (UserInputDataTypeKey*)data;
				Usart0TransmitStr("KEY");
				Usart0Transmit(KeyPad_GetKeyNumInt(dataKey->Key));
				break;
			}
			case USERDATA_ENCODER: 
			{	
				UserInputDataTypeEncoder* dataEnc = (UserInputDataTypeEncoder*)data;
				if(dataEnc->EncDir == ENCCLOCKWISE) { Usart0TransmitStr("ENC_CW"); }
				else { Usart0TransmitStr("ENC_CCW"); }
	
				break;
			}
			case USERDATA_ENCODER_BUTTON:
			{
				Usart0TransmitStr("ENC_PB");
				break;
			}
			case USERDATA_USART: 
			{
				UserInputDataTypeUsart* dataUsart = (UserInputDataTypeUsart*)data;
				Usart0TransmitStr("D");
				Usart0Transmit(dataUsart->Data);
				break;
			}
			default: break;
		}
	}
}