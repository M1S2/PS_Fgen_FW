/*
 * Pins.h
 *
 * Created: 21.08.2020 22:38:54
 *  Author: V17
 */ 


#ifndef PINS_H_
#define PINS_H_

#define F_CPU 16000000

#include <util/delay.h>
#include <avr/io.h>

#define SET_BIT(x, y) x |= (1 << y);
#define CLEAR_BIT(x, y) x &= ~(1 << y);
#define TOGGLE_BIT(x, y) x ^= (1 << y);


// ADC Definitions
#define ADC_DVM2	PORTA7
#define ADC_DVM1	PORTA6
#define ADC_3V3		PORTA5
#define ADC_5V		PORTA4
#define ADC_12V		PORTA3
#define ADC_12V_NEG	PORTA2
#define ADC_PS_VOLT	PORTA1
#define ADC_PS_CUR	PORTA0

// Chip select pins
#define SPI_SCK		PORTB7
#define SPI_MISO	PORTB6
#define SPI_MOSI	PORTB5
#define PS_CS		PORTB4		// Is SPI_SS line and must be output for master mode
#define DDS_CS		PORTB3
#define DDS_LDAC	PORTB2
#define LCD_CS		PORTB1
#define LCD_A0		PORTB0

// Keys Definitions
#define KEY_R4		PORTC7
#define KEY_R3		PORTC6
#define KEY_R2		PORTC5
#define KEY_R1		PORTC4
#define KEY_C4		PORTC3
#define KEY_C3		PORTC2
#define KEY_C2		PORTC1
#define KEY_C1		PORTC0

// PORTD6 and PORTD7 not used
#define ATX_PWR_ON	PORTD5
#define ENC_PB		PORTD4
#define ENC_B		PORTD3
#define ENC_A		PORTD2
#define USB_TX		PORTD1
#define USB_RX		PORTD0


// Initialize the Pins
void Pins_Init();

#endif /* PINS_H_ */