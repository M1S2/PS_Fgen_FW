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
	float ATX_3V3;
	float ATX_5V;
	float ATX_12V;
	float ATX_12V_NEG;
	float PS_VOLT;
	float PS_CURR;
	float PS_POWER;
	float DMM1;
	float DMM2;
};

extern DevStatus_t DevStatus;		// defined in main.cpp

#endif /* DEVSTATUS_H_ */