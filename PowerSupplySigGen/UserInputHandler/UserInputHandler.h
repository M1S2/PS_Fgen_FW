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
#include "../Encoder/Encoder.h"
#include <stdbool.h>

/* Enum used to differentiate the user input types */
typedef enum UserInputDataTypes
{
	USERDATA_KEY,
	USERDATA_ENCODER,
	USERDATA_ENCODER_BUTTON,
	USERDATA_USART
} UserInputDataTypes_t;

/* Class that holds one user input made by keys, encoder or USART */
class UserInputData
{
	public:
		UserInputDataTypes_t DataType;
		Keys_t Key;
		EncoderDirection_t EncDir;
		bool EncBtn;
		uint8_t UsartChr;
	
		UserInputData() 
		{}
	
		UserInputData(Keys_t key) : Key(key)
		{
			DataType = USERDATA_KEY;
		}
		
		UserInputData(EncoderDirection_t encDir) : EncDir(encDir)
		{
			DataType = USERDATA_ENCODER;
		}
		
		UserInputData(bool encBtn) : EncBtn(encBtn)
		{
			DataType = USERDATA_ENCODER_BUTTON;
		}
		
		UserInputData(uint8_t usartChr) : UsartChr(usartChr)
		{
			DataType = USERDATA_USART;
		}
};


class UserInputHandlerClass 
{
	private:
		CircularBuffer<UserInputData, 1024> _userInputRingBuffer;
		
	public:		
		void EnqueueKeyInput(Keys_t userKeyInput);
		void EnqueueEncoderInput(EncoderDirection_t userEncoderInput);		
		void EnqueueEncoderButtonInput();
		void EnqueueUsartInput(uint8_t userDataInput);
		
		void ProcessInputs();
};

extern UserInputHandlerClass UserInputHandler;

#endif /* USERINPUTHANDLER_H_ */