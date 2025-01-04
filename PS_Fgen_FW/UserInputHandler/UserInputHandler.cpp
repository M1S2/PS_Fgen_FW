/*
 * UserInputHandler.cpp
 * Created: 08.09.2020 19:31:42
 * Author: Markus Scheich
 */ 

#include "UserInputHandler.h"

#include "../Device.h"
#include "../SCPI/SCPI_Device.h"
#include "../Device.h"

void UserInputHandlerClass::ProcessInputs()
{
	while(!_userInputRingBuffer.empty())
	{
		UserInputData* data = _userInputRingBuffer.dequeue();

		if(!Device.IsUserInputLocked() || data->DataType == USERDATA_USART || data->DataType == USERDATA_ON_OFF_BUTTONS)
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
					Serial.write(data->UsartChr);

					#ifdef SCPI_ENABLED
						SCPI_Input(&scpi_context, (char*)&data->UsartChr, 1);
					#endif
					
					break;
				}
				case USERDATA_ON_OFF_BUTTONS:
				{
					OnOffControls_DoButtonAction(data->OnOffButton);										
					break;
				}
			#ifdef TOUCH_ENABLED
				case USERDATA_TOUCH:
				{
					Device.ScreenManager.TouchInput(data->TouchX, data->TouchY, data->TouchType);
					break;
				}
			#endif
				default: break;
			}
		}
		else if(Device.IsUserInputLocked() && data->DataType != USERDATA_USART)
		{
			Device.UpdateControlStateOnUserInput();
		}
		
		Device.ScreenManager.RedrawScreenRequest = true;		// Generate a redraw request, whenever a user input occurs (no matter if key or USART)
	}
}