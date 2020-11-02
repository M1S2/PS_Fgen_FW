/*
 * UserInputHandler.h
 *
 * Created: 08.09.2020 19:31:51
 *  Author: V17
 */ 


#ifndef USERINPUTHANDLER_H_
#define USERINPUTHANDLER_H_

#include "UserInputDataTypes.h"
#include "CircularBuffer.h"
#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"

class UserInputHandlerClass 
{
	private:
		CircularBuffer<UserInputDataTypesBase> _userInputRingBuffer;
		
	public:		
		void EnqueueKeyInput(Keys_t userKeyInput);
		void EnqueueEncoderInput(EncoderDirection_t userEncoderInput);		
		void EnqueueEncoderButtonInput();
		void EnqueueUsartInput(uint8_t userDataInput);
		
		void ProcessInputs();
};

extern UserInputHandlerClass UserInputHandler;

#endif /* USERINPUTHANDLER_H_ */