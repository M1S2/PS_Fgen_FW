/*
 * UserInputDataTypes.h
 *
 * Created: 31.10.2020 17:46:57
 *  Author: V17
 */ 


#ifndef USERINPUTDATATYPES_H_
#define USERINPUTDATATYPES_H_

#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"
#include <stdbool.h>

typedef enum UserInputDataTypes
{
	USERDATA_KEY,
	USERDATA_ENCODER,
	USERDATA_ENCODER_BUTTON,
	USERDATA_USART
} UserInputDataTypes_t;

class UserInputDataTypesBase
{
	public:
		UserInputDataTypes_t DataType;
		
		UserInputDataTypesBase() 
		{}
			
		UserInputDataTypesBase(UserInputDataTypes_t dataType)
		{
			DataType = dataType;			
		}
};

class UserInputDataTypeKey : public UserInputDataTypesBase
{
	public:
		Keys_t Key;
	
		UserInputDataTypeKey() : UserInputDataTypesBase(USERDATA_KEY)
		{}
			
		UserInputDataTypeKey(Keys_t key) : UserInputDataTypesBase(USERDATA_KEY)
		{
			Key = key;
		}
};

class UserInputDataTypeEncoder : public UserInputDataTypesBase
{
	public:
		EncoderDirection_t EncDir;

		UserInputDataTypeEncoder() : UserInputDataTypesBase(USERDATA_ENCODER)
		{}
	
		UserInputDataTypeEncoder(EncoderDirection_t encDir) : UserInputDataTypesBase(USERDATA_ENCODER)
		{
			EncDir = encDir;
		}
};

class UserInputDataTypeUsart : public UserInputDataTypesBase
{
	public:
		uint8_t Data;
	
		UserInputDataTypeUsart() : UserInputDataTypesBase(USERDATA_USART)
		{}
		
		UserInputDataTypeUsart(uint8_t data) : UserInputDataTypesBase(USERDATA_USART)
		{
			Data = data;
		}
};

#endif /* USERINPUTDATATYPES_H_ */