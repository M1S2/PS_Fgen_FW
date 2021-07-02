/**
 * @file	ADC.h
 * @date	15.09.2020 19:30:30
 * @author	V17
 * @brief	Containing the Analog Digital Converter part.
 *
 * see: https://www.newbiehack.com/MicrocontrollersADCReadingMultipleAnalogVolageSources.aspx
 */ 

#ifndef ADC_H_
#define ADC_H_

#include "../Pins/Pins.h"

/**
 * @brief Initialize the Analog Digital Converter.
 * This function initializes all neccessary registers of the ADC.
 * After ADC_init() enable Interrupts with sei();
 * Then start first conversion with ADC_startConversion();
 */
void ADC_init();

/**
 * @brief Start the conversion of the Analog Digital Converter.
 * This function starts the conversion starting with channel 0.
 * It is only neccessary to call this function once. 
 * All further conversions are automatically started after the last one finished.
 */
void ADC_startConversion();


#endif /* ADC_H_ */