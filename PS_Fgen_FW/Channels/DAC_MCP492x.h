/**
 * @file	DAC_MCP492x.h
 * @date	31.08.2020 18:43:02
 * @author	Markus Scheich
 * @brief	Containing the functions to control the MCP492x DACs.
 */ 

#ifndef DAC_MCP492x_H_
#define DAC_MCP492x_H_

#include "../Pins/Pins.h"
#include "../Configuration.h"

#define MCP492X_DACB				7				/**< Position of the DACA or DACB Select bit in the write command register */
#define MCP492X_BUFFERED			6				/**< Position of the VREF Input Buffer Control bit in the write command register */
#define MCP492X_GAIN_SELECT_SINGLE	5				/**< Position of the Output Gain Select bit in the write command register */
#define MCP492X_SHDN				4				/**< Position of the Output Power Down Control bit in the write command register */

#define SELECT_MCP4922		CLEAR_BIT(PORTB, DDS_CS);				/**< Select the MCP4922 DAC by pulling the DDS_CS pin low */
#define DESELECT_MCP4922	SET_BIT(PORTB, DDS_CS);					/**< Deselect the MCP4922 DAC by pulling the DDS_CS pin high */
#define IS_MCP4922_SELECTED	BIT_IS_CLEARED(PORTB, DDS_CS);			/**< Check if the MCP4922 DAC is selected by reading the DDS_CS pin */
#define SELECT_MCP4921		CLEAR_BIT(PORTB, PS_CS);				/**< Select the MCP4921 DAC by pulling the PS_CS pin low */
#define DESELECT_MCP4921	SET_BIT(PORTB, PS_CS);					/**< Deselect the MCP4921 DAC by pulling the PS_CS pin high */
#define IS_MCP4921_SELECTED	BIT_IS_CLEARED(PORTB, PS_CS);			/**< Check if the MCP4921 DAC is selected by reading the PS_CS pin */
#define CLEAR_LDAC_MCP4922	CLEAR_BIT(PORTB, DDS_LDAC);				/**< Clear the latch pin of the MCP4922 DAC */
#define SET_LDAC_MCP4922	SET_BIT(PORTB, DDS_LDAC);				/**< Set the latch pin of the MCP4922 DAC */

/**
 * Disable the latching of the MCP4922 device (two channels).
 */
void MCP4922_DisableLatching();

/**
 * Latch the outputs of the MCP4922 device (two channels).
 */
void MCP4922_LatchOutput();

/**
 * Set the DAC data value of the MCP4921 device.
 * @param dac_data DAC data that is written to the write command registers. Only the lower 12 bits are used.
 */
void MCP4921_DAC_Set(uint16_t dac_data);

/**
 * Set the voltage of the MCP4921 device.
 * The voltage is converted to the DAC data using the calibrated reference voltage of the device.
 * @param voltage Set the MCP4921 device to this voltage.
 */
void MCP4921_Voltage_Set(float voltage);

/**
 * Set the DAC data value of the MCP4922 device.
 * @param dac_data DAC data that is written to the write command registers. Only the lower 12 bits are used.
 * @param channel_A_B Set this to 'B' to set the value of the second channel (B). All other characters set the value of the first channel (A).
 */
void MCP4922_DAC_Set(uint16_t dac_data, char channel_A_B);

/**
 * Set the voltage of the MCP4922 device.
 * The voltage is converted to the DAC data using the calibrated reference voltage of the device.
 * @param voltage Set the MCP4922 device to this voltage.
 * @param channel_A_B Set this to 'B' to set the value of the second channel (B). All other characters set the value of the first channel (A).
 */
void MCP4922_Voltage_Set(float voltage, char channel_A_B);

#endif /* DAC_MCP492x_H_ */