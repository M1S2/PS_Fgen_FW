/*
 * PowerSupply.cpp
 *
 * Created: 23.09.2020 19:47:47
 *  Author: V17
 */ 

#include "PowerSupply.h"

void PS_Output_Set()
{
	if(DevSettings.PS_Output_Enabled)
	{
		uint32_t voltage_mV = ((DevSettings.PS_Load_Impedance + PS_INTERNAL_IMPEDANCE) / (float)DevSettings.PS_Load_Impedance) * DevSettings.PS_Voltage_mV;				//Vset = ((Rload + Rinternal) / Rload) * Vout		
		MCP4921_Voltage_Set(voltage_mV / 2);		// divided by two because of OpAmp in circuit that has an amplification of 2
	}
	else
	{
		MCP4921_Voltage_Set(0);
	}
}