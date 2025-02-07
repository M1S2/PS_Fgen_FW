/*
 * DAC_MCP492x.cpp
 * Created: 31.08.2020 18:42:53
 * Author: Markus Scheich
 * see: https://github.com/murjay/MCP4922-DAC-Driver/blob/master/MCP4922_DAC_driver.c
 */ 

#include "DAC_MCP492x.h"
#include "../Spi/spi.h"
#include "../Device.h"

void MCP4921_DAC_Set(uint16_t dac_data)
{		
	uint8_t low_byte=0, high_byte=0;
	high_byte |= (1 << MCP492X_SHDN);					/*Set SHDN bit high for DAC A active operation*/
	high_byte |= (1 << MCP492X_BUFFERED);				/*Enable buffered inputs for Vref*/
	high_byte |= (1 << MCP492X_GAIN_SELECT_SINGLE);		/*Select single gain*/

	high_byte |= ((dac_data >> 8) & 0x0F);
	low_byte = (dac_data & 0xFF);

	SPI_SelectDevice(SPI_DEV_PS);

	/*send the word*/
	SPI_SendByte(high_byte);
	SPI_SendByte(low_byte);

	SPI_SelectDevice(SPI_DEV_TFT);
}

void MCP4921_Voltage_Set(float voltage)
{
	//VOUT = (GAIN * VREF * D/4096)
	uint16_t dac_data;
	if(voltage > Device.CalibrationFactors.Cal_RefVoltage)
	{
		dac_data = 4095;		// Maximum value for 12-bit DAC
	}
	else
	{
		dac_data = (uint16_t)(voltage * (4095.0f / Device.CalibrationFactors.Cal_RefVoltage));
	}
	MCP4921_DAC_Set(dac_data);
}

void MCP4922_DAC_Set(uint16_t dac_data, char channel_A_B)
{			
	uint8_t low_byte=0, high_byte=0;
	if(channel_A_B == 'B')
	{
		high_byte |= (1 << MCP492X_DACB);				/*Set bit to select DAC B*/
	}
	high_byte |= (1 << MCP492X_SHDN);					/*Set SHDN bit high for DAC A active operation*/
	high_byte |= (1 << MCP492X_BUFFERED);				/*Enable buffered inputs for Vref*/
	high_byte |= (1 << MCP492X_GAIN_SELECT_SINGLE);		/*Select single gain*/

	high_byte |= ((dac_data >> 8) & 0x0F);
	low_byte = (dac_data & 0xFF);

	SPI_SelectDevice(SPI_DEV_DDS);

	/*send the word*/
	SPI_SendByte(high_byte);
	SPI_SendByte(low_byte);

	SPI_SelectDevice(SPI_DEV_TFT);
}

void MCP4922_Voltage_Set(float voltage, char channel_A_B)
{
	//VOUT = (GAIN * VREF * D/4096)
	voltage = (voltage + 10) / 4;
	MCP4922_DAC_Set((uint16_t)(voltage * (4095.0f / Device.CalibrationFactors.Cal_RefVoltage)), channel_A_B);
}