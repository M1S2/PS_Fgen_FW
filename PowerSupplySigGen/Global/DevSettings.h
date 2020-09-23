/*
 * DevSettings.h
 *
 * Created: 15.09.2020 19:33:01
 *  Author: V17
 */ 


#ifndef DEVSETTINGS_H_
#define DEVSETTINGS_H_

#define AVR_VCC		5.27
#define AVR_VCC_MV	5270

struct DevSettings_t
{
	uint8_t TabIndex;
	uint16_t PS_Voltage_mV;
	uint8_t PS_Output_Enabled;
};

extern DevSettings_t DevSettings;		// defined in main.cpp

#endif /* DEVSETTINGS_H_ */