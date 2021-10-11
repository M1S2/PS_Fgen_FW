/**
 * @file	Pins.h
 * @date	21.08.2020 22:38:54
 * @author	Markus Scheich
 * @brief	Containing defines and functions for basic pin initialization and handling.
 */ 

#ifndef PINS_H_
#define PINS_H_

#define F_CPU 16000000									/**< CPU clock frequency */

#include <util/delay.h>
#include <avr/io.h>

#define SET_BIT(x, y) x |= (1 << y);					/**< Set the bit at position y in register x */
#define CLEAR_BIT(x, y) x &= ~(1 << y);					/**< Clear the bit at position y in register x */
#define TOGGLE_BIT(x, y) x ^= (1 << y);					/**< Toggle the bit at position y in register x */

#define BIT_IS_SET(x, y) (x & (1 << y)) == (1 << y)		/**< Check if the bit at position y in register x is set */
#define BIT_IS_CLEARED(x, y) (x & (1 << y)) == 0		/**< Check if the bit at position y in register x is cleared */


// ADC Definitions
#define ADC_DVM2	PORTA7			/**< Bit position of the digital multimeter channel 1 ADC input */
#define ADC_DVM1	PORTA6			/**< Bit position of the digital multimeter channel 2 ADC input */
#define ADC_3V3		PORTA5			/**< Bit position of the 3.3V ADC input */
#define ADC_5V		PORTA4			/**< Bit position of the 5V ADC input */
#define ADC_12V		PORTA3			/**< Bit position of the 12V ADC input */
#define ADC_12V_NEG	PORTA2			/**< Bit position of the -12V ADC input */
#define ADC_PS_VOLT	PORTA1			/**< Bit position of the power supply voltage ADC input */
#define ADC_PS_CUR	PORTA0			/**< Bit position of the power supply current ADC input */

// Chip select pins
#define SPI_SCK		PORTB7			/**< Bit position of the SPI clock */
#define SPI_MISO	PORTB6			/**< Bit position of the SPI MISO line */
#define SPI_MOSI	PORTB5			/**< Bit position of the SPI MOSI line */
#define PS_CS		PORTB4			/**< Bit position of the power supply chip select line (Is SPI_SS line and must be output for master mode) */
#define DDS_CS		PORTB3			/**< Bit position of the direct digital synthesis chip select line */
#define DDS_LDAC	PORTB2			/**< Bit position of the direct digital synthesis latch line */
#define LCD_CS		PORTB1			/**< Bit position of the LCD chip select line */
#define LCD_A0		PORTB0			/**< Bit position of the LCD address line */

// Keys Definitions
#define KEY_R4		PORTC7			/**< Bit position of the KeyPad row 4 line */
#define KEY_R3		PORTC6			/**< Bit position of the KeyPad row 3 line */
#define KEY_R2		PORTC5			/**< Bit position of the KeyPad row 4 line */
#define KEY_R1		PORTC4			/**< Bit position of the KeyPad row 1 line */
#define KEY_C4		PORTC3			/**< Bit position of the KeyPad column 4 line */
#define KEY_C3		PORTC2			/**< Bit position of the KeyPad column 3 line */
#define KEY_C2		PORTC1			/**< Bit position of the KeyPad column 2 line */
#define KEY_C1		PORTC0			/**< Bit position of the KeyPad column 1 line */

// PORTD6 and PORTD7 not used (PORTD5 used as test pin)
#define ENC_PB		PORTD4			/**< Bit position of the Encoder push button line */
#define ENC_B		PORTD3			/**< Bit position of the Encoder B line */
#define ENC_A		PORTD2			/**< Bit position of the Encoder A line */
#define USB_TX		PORTD1			/**< Bit position of the UART TX line */
#define USB_RX		PORTD0			/**< Bit position of the UART RX line */

#define TEST_PIN	PORTD5			/**< Bit position of the pin used as test pin for different debugging purposes */

/**
 * Initialize all pins (directions and states).
 */
void Pins_Init();

#endif /* PINS_H_ */