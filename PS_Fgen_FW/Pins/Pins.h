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

#define SET_REG_BIT(x, y) x |= (1 << y);					/**< Set the bit at position y in register x */
#define CLEAR_REG_BIT(x, y) x &= ~(1 << y);					/**< Clear the bit at position y in register x */

// Chip select pin bit positions (They are used for faster access than the arduino digital write methods)
#define BIT_POS_TS_CS		PORTA3		/**< Bit position of the touch screen chip select */
#define BIT_POS_TFT_CS		PORTB1		/**< Bit position of the TFT chip select line */
#define BIT_POS_IO_EXP_CS	PORTB2		/**< Bit position of the on/off controls IO expander chip select line */
#define BIT_POS_DDS_CS		PORTB3		/**< Bit position of the direct digital synthesis chip select line */
#define BIT_POS_PS_CS		PORTB4		/**< Bit position of the power supply chip select line (Is SPI_SS line and must be output for master mode) */

// https://camo.githubusercontent.com/331d26748c79ccaf42bb0837711b14728c82e47e6fd21b2997a1b9c5155cf672/68747470733a2f2f692e696d6775722e636f6d2f7a424e506335622e706e67
#define PIN_NUMBER_TFT_A0	    0		/**< Arduino pin number for the TFT A0 pin (PORTB0) */
#define PIN_NUMBER_TFT_CS	    1		/**< Arduino pin number for the TFT chip select (PORTB1) */
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

/*********************************
 * SPI Chip selects
 *********************************/
#define SPI_SELECT_TOUCH	    CLEAR_REG_BIT(PORTA, BIT_POS_TS_CS)		        /**< Select the touch screen by pulling the CS pin low */
#define SPI_DESELECT_TOUCH	    SET_REG_BIT(PORTA, BIT_POS_TS_CS)	            /**< Deselect the touch screen by pulling the CS pin high */

#define SPI_SELECT_TFT		    CLEAR_REG_BIT(PORTB, BIT_POS_TFT_CS)	        /**< Select the TFT by pulling the TFT_CS pin low */
#define SPI_DESELECT_TFT	    SET_REG_BIT(PORTB, BIT_POS_TFT_CS)	            /**< Deselect the TFT by pulling the TFT_CS pin high */

#define SPI_SELECT_DDS		    CLEAR_REG_BIT(PORTB, BIT_POS_DDS_CS)		    /**< Select the DDS DAC by pulling the DDS_CS pin low */
#define SPI_DESELECT_DDS	    SET_REG_BIT(PORTB, BIT_POS_DDS_CS)		        /**< Deselect the DDS DAC by pulling the DDS_CS pin high */

#define SPI_SELECT_PS		    CLEAR_REG_BIT(PORTB, BIT_POS_PS_CS)	            /**< Select the PS DAC by pulling the PS_CS pin low */
#define SPI_DESELECT_PS	        SET_REG_BIT(PORTB, BIT_POS_PS_CS)               /**< Deselect the PS DAC by pulling the PS_CS pin high */

#define SPI_SELECT_IO_EXP		CLEAR_REG_BIT(PORTB, BIT_POS_IO_EXP_CS)	        /**< Select the IO expander by pulling the IO_EXP_CS pin low */
#define SPI_DESELECT_IO_EXP	    SET_REG_BIT(PORTB, BIT_POS_IO_EXP_CS)	        /**< Deselect the IO expander by pulling the IO_EXP_CS pin high */

#define BITMASK_CS_PORTA        (1 << BIT_POS_TS_CS)                                                                                /**< Bitmask for the PortA SPI chip selects (1s mark chip select positions) */
#define BITMASK_CS_PORTB        (1 << BIT_POS_PS_CS) | (1 << BIT_POS_DDS_CS) | (1 << BIT_POS_IO_EXP_CS) | (1 << BIT_POS_TFT_CS)     /**< Bitmask for the PortB SPI chip selects (1s mark chip select positions) */
#define SPI_DESELECT_ALL  	    PORTA |= BITMASK_CS_PORTA; PORTB |= BITMASK_CS_PORTB;                                               /**< Disable all SPI chip selects (using direct port accesses) */

#endif /* PINS_H_ */