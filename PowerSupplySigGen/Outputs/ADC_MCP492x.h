/*
 * ADC_MCP492x.h
 *
 * Created: 31.08.2020 18:43:02
 *  Author: V17
 */ 


#ifndef ADC_MCP492x_H_
#define ADC_MCP492x_H_

#include "../Pins/Pins.h"

#define MCP492X_DACB				7
#define MCP492X_BUFFERED			6
#define MCP492X_GAIN_SELECT_SINGLE	5
#define MCP492X_SHDN				4

#define SELECT_MCP4922		CLEAR_BIT(PORTB, DDS_CS);
#define DESELECT_MCP4922	SET_BIT(PORTB, DDS_CS);
#define SELECT_MCP4921		CLEAR_BIT(PORTB, PS_CS);
#define DESELECT_MCP4921	SET_BIT(PORTB, PS_CS);
#define CLEAR_LDAC_MCP4922	CLEAR_BIT(PORTB, DDS_LDAC);
#define SET_LDAC_MCP4922	SET_BIT(PORTB, DDS_LDAC);

void MCP4922_DisableLatching();
void MCP4922_LatchOutput();
void MCP4921_DAC_Set(uint16_t dac_data);
void MCP4921_Voltage_Set(int voltage_mV);
void MCP4922_DAC_Set(uint16_t dac_data, char channel_A_B);
void MCP4922_Voltage_Set(int voltage_mV, char channel_A_B);

#endif /* ADC_MCP492x_H_ */