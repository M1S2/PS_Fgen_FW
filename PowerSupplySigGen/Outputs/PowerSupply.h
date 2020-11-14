/*
 * PowerSupply.h
 *
 * Created: 23.09.2020 19:47:56
 *  Author: V17
 */ 


#ifndef POWERSUPPLY_H_
#define POWERSUPPLY_H_

#include "ADC_MCP492x.h"

#define PS_INTERNAL_IMPEDANCE	5.7
#define PS_MIN_LOAD_IMPEDANCE	1.0

class PowerSupplyClass
{
	public:
		float Voltage;
		bool OutputEnabled;
		float LoadImpedance;
		
		PowerSupplyClass() 
		{}
			
		void UpdateOutput();
};

extern PowerSupplyClass PowerSupply;

void PSUpdateOutputCallbackFunction();

#endif /* POWERSUPPLY_H_ */