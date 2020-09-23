/*
 * DevStatus.h
 *
 * Created: 15.09.2020 19:32:28
 *  Author: V17
 */ 


#ifndef DEVSTATUS_H_
#define DEVSTATUS_H_

struct DevStatus_t
{
	uint16_t ATX_3V3_mV;
	uint16_t ATX_5V_mV;
	uint16_t ATX_12V_mV;
	uint16_t ATX_12V_NEG_mV;
	uint16_t PS_VOLT_mV;
	uint16_t PS_CURR_mV;
	uint16_t DMM1_mV;
	uint16_t DMM2_mV;
};

extern DevStatus_t DevStatus;		// defined in main.cpp

#endif /* DEVSTATUS_H_ */