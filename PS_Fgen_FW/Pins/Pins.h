/**
 * @file	Pins.h
 * @date	21.08.2020 22:38:54
 * @author	Markus Scheich
 * @brief	Containing defines and functions for basic pin initialization and handling.
 */ 

#ifndef PINS_H_
#define PINS_H_

#include <Arduino.h>
#include <util/delay.h>
#include <avr/io.h>

// https://camo.githubusercontent.com/331d26748c79ccaf42bb0837711b14728c82e47e6fd21b2997a1b9c5155cf672/68747470733a2f2f692e696d6775722e636f6d2f7a424e506335622e706e67
#define PIN_NUMBER_LCD_A0	    0		/**< Arduino pin number for the LCD A0 pin (PORTB0) */
#define PIN_NUMBER_LCD_CS	    1		/**< Arduino pin number for the LCD chip select (PORTB1) */
#define PIN_NUMBER_IO_EXP_CS	2		/**< Arduino pin number for the on/off controls IO expander chip select line (PORTB2) */
#define PIN_NUMBER_DDS_CS	    3		/**< Arduino pin number for the direct digital synthesis chip select line (PORTB3) */
#define PIN_NUMBER_PS_CS	    4		/**< Arduino pin number for the power supply chip select line (Is SPI_SS line and must be output for master mode) (PORTB4) */

#define PIN_NUMBER_USB_RX       8		/**< Arduino pin number UART RX line (PORTD0) */
#define PIN_NUMBER_USB_TX       9		/**< Arduino pin number UART TX line (PORTD1) */
#define PIN_NUMBER_ENC_A        10		/**< Arduino pin number for the Encoder A line (PORTD2) */
#define PIN_NUMBER_ENC_B        11		/**< Arduino pin number for the Encoder B line (PORTD3) */
#define PIN_NUMBER_ENC_PB       12		/**< Arduino pin number for the Encoder push button line (PORTD4) */
#define PIN_NUMBER_DMM1_NEG     13		/**< Arduino pin number for the DMM1 negative voltage signal pin (PORTD5) */
#define PIN_NUMBER_DMM2_NEG     14		/**< Arduino pin number for the DMM2 negative voltage signal pin (PORTD6) */
#define PIN_NUMBER_IO_EXP_INT   15		/**< Arduino pin number for the on/off controls IO expander interrupt pin (PORTD7) */

// Caution: The key pin numbers are not used. In the KeyPad.h file the complete port C is used directly.
#define PIN_NUMBER_KEY_C1	    16		/**< Arduino pin number for the KeyPad column 1 line (PORTC0) */
#define PIN_NUMBER_KEY_C2	    17		/**< Arduino pin number for the KeyPad column 2 line (PORTC1) */
#define PIN_NUMBER_KEY_C3	    18		/**< Arduino pin number for the KeyPad column 3 line (PORTC2) */
#define PIN_NUMBER_KEY_C4	    19		/**< Arduino pin number for the KeyPad column 4 line (PORTC3) */
#define PIN_NUMBER_KEY_R1	    20		/**< Arduino pin number for the KeyPad row 1 line (PORTC4) */
#define PIN_NUMBER_KEY_R2	    21		/**< Arduino pin number for the KeyPad row 2 line (PORTC5) */
#define PIN_NUMBER_KEY_R3	    22		/**< Arduino pin number for the KeyPad row 3 line (PORTC6) */
#define PIN_NUMBER_KEY_R4	    23		/**< Arduino pin number for the KeyPad row 4 line (PORTC7) */

#define PIN_NUMBER_ADC_PS_CUR   24		/**< Arduino pin number for the power supply current ADC input (PORTA0) */
#define PIN_NUMBER_ADC_PS_VOLT  25		/**< Arduino pin number for the power supply voltage ADC input (PORTA1) */
#define PIN_NUMBER_TOUCH_IRQ    26		/**< Arduino pin number for the touch screen IRQ pin (PORTA2) */
#define PIN_NUMBER_TOUCH_CS	    27		/**< Arduino pin number for the touch screen CS pin (PORTA3) */
#define PIN_NUMBER_ADC_5V	    28		/**< Arduino pin number for the 5V ADC input (PORTA4) */
#define PIN_NUMBER_ADC_3V3	    29		/**< Arduino pin number for the 3.3V ADC input (PORTA5) */
#define PIN_NUMBER_ADC_DVM1	    30		/**< Arduino pin number for the digital multimeter channel 1 ADC input (PORTA6) */
#define PIN_NUMBER_ADC_DVM2	    31		/**< Arduino pin number for the digital multimeter channel 2 ADC input (PORTA7) */

/**
 * Initialize all pins (directions and states).
 */
void Pins_Init();

#define SPI_SELECT_TOUCH	    digitalWrite(PIN_NUMBER_TOUCH_CS, LOW);		/**< Select the touch screen by pulling the CS pin low */
#define SPI_DESELECT_TOUCH	    digitalWrite(PIN_NUMBER_TOUCH_CS, HIGH);	/**< Deselect the touch screen by pulling the CS pin high */

#define SPI_SELECT_LCD		    digitalWrite(PIN_NUMBER_LCD_CS, LOW);	    /**< Select the LCD by pulling the LCD_CS pin low */
#define SPI_DESELECT_LCD	    digitalWrite(PIN_NUMBER_LCD_CS, HIGH);	    /**< Deselect the LCD by pulling the LCD_CS pin high */

#define SPI_SELECT_DDS		    digitalWrite(PIN_NUMBER_DDS_CS, LOW);		/**< Select the DDS DAC by pulling the DDS_CS pin low */
#define SPI_DESELECT_DDS	    digitalWrite(PIN_NUMBER_DDS_CS, HIGH);		/**< Deselect the DDS DAC by pulling the DDS_CS pin high */
#define SPI_IS_DDS_SELECTED	    digitalRead(PIN_NUMBER_DDS_CS) == LOW		/**< Check if the DDS DAC is selected by reading the DDS_CS pin */

#define SPI_SELECT_PS		    digitalWrite(PIN_NUMBER_PS_CS, LOW);	    /**< Select the PS DAC by pulling the PS_CS pin low */
#define SPI_DESELECT_PS	        digitalWrite(PIN_NUMBER_PS_CS, HIGH);       /**< Deselect the PS DAC by pulling the PS_CS pin high */
#define SPI_IS_PS_SELECTED	    digitalRead(PIN_NUMBER_PS_CS) == LOW	    /**< Check if the PS DAC is selected by reading the PS_CS pin */

#define SPI_SELECT_IO_EXP		digitalWrite(PIN_NUMBER_IO_EXP_CS, LOW);	/**< Select the IO expander by pulling the IO_EXP_CS pin low */
#define SPI_DESELECT_IO_EXP	    digitalWrite(PIN_NUMBER_IO_EXP_CS, HIGH);	/**< Deselect the IO expander by pulling the IO_EXP_CS pin high */

#define SPI_DESELECT_ALL        SPI_DESELECT_TOUCH   SPI_DESELECT_PS   SPI_DESELECT_DDS   SPI_DESELECT_IO_EXP   SPI_DESELECT_LCD    /**< Disable all SPI chip selects */

#endif /* PINS_H_ */