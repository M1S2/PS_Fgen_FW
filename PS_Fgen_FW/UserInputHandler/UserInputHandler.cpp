/*
 * UserInputHandler.cpp
 * Created: 08.09.2020 19:31:42
 * Author: Markus Scheich
 */ 

#include "UserInputHandler.h"

#include "../USART/USART.h"
#include "../Device.h"
#include "../SCPI/SCPI_Device.h"
#include "../Device.h"

void UserInputHandlerClass::EnqueueKeyInput(Keys_t userKeyInput)
{
	if(!_userInputRingBuffer.full())
	{
		UserInputData keyInput(userKeyInput);
		_userInputRingBuffer.enqueue(&keyInput);
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

		if(!Device.IsUserInputLocked() || data->DataType == USERDATA_USART)
		{		
			switch(data->DataType)
			{
				case USERDATA_KEY: 
				{
					Device.ScreenManager.KeyInput(data->Key);
					break;
				}
				case USERDATA_USART: 
				{
					if(Device.SerialEchoEnabled) { Usart0Transmit(data->UsartChr); }	// Echo character if enabled
					
					#ifdef SCPI_ENABLED
						SCPI_Input(&scpi_context, (char*)&data->UsartChr, 1);
					#endif
					
					break;
				}
				default: break;
			}
		}
		else if(Device.IsUserInputLocked() && data->DataType != USERDATA_USART)
		{
			Device.UpdateControlStateOnUserInput();
		}
	}
}