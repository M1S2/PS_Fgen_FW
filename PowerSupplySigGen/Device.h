/*
 * Device.h
 *
 * Created: 06.12.2020 14:44:34
 *  Author: V17
 */ 


#ifndef DEVICE_H_
#define DEVICE_H_

#include <avr/io.h>

typedef enum DeviceControlStates
{
	DEV_CTRL_LOCAL,
	DEV_CTRL_REMOTE,
	DEV_CTRL_RWLOCK
}DeviceControlStates_t; 

class DeviceClass
{
	private:
	
	public:
		DeviceControlStates_t DeviceControlState;
		uint32_t SerialBaudRate;
		bool SerialEchoEnabled;
	
		DeviceClass()
		{
			DeviceControlState = DEV_CTRL_LOCAL;
		}
		
		bool IsUserInputLocked();
		void UpdateControlStateOnUserInput();
		
		void SetSerialBaudRate(uint32_t baud);
		void SetSerialEchoEnabled(bool echoEnabled);
};

extern DeviceClass Device;

#endif /* DEVICE_H_ */