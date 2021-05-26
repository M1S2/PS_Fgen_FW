/*
 * UserInputHandler.h
 *
 * Created: 08.09.2020 19:31:51
 *  Author: V17
 */ 


#ifndef USERINPUTHANDLER_H_
#define USERINPUTHANDLER_H_

#include "CircularBuffer.h"
#include "../KeyPad/KeyPad.h"
#include <stdbool.h>

#include "../Configuration.h"

#define USERINPUT_QUEUE_FULL_MSG	"The UserInput queue is full.\nThe device couldn't keep up with processing.\nPlease wait until this message disappears\nbefore sending new inputs."

/* Enum used to differentiate the user input types */
typedef enum UserInputDataTypes
{
	USERDATA_KEY,
	USERDATA_USART
} UserInputDataTypes_t;

/* Class that holds one user input made by keys, encoder or USART */
class UserInputData
{
	public:
		UserInputDataTypes_t DataType;
		Keys_t Key;
		uint8_t UsartChr;
	
		UserInputData() 
		{}
	
		UserInputData(Keys_t key) : Key(key)
		{
			DataType = USERDATA_KEY;
		}
			
		UserInputData(uint8_t usartChr) : UsartChr(usartChr)
		{
			DataType = USERDATA_USART;
		}
};


class UserInputHandlerClass 
{
	private:
		CircularBuffer<UserInputData, USERINPUT_QUEUE_LENGTH> _userInputRingBuffer;
		
	public:		
		void EnqueueKeyInput(Keys_t userKeyInput);
		void EnqueueUsartInput(uint8_t userDataInput);
		
		void ProcessInputs();
};

#endif /* USERINPUTHANDLER_H_ */