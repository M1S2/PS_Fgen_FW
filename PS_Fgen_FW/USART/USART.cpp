/*
 * USART.cpp
 * Created: 28.10.2020 17:14:47
 * Author: Markus Scheich
 */ 

#include "USART.h"
#include "../Device.h"

ISR (USART0_RX_vect)
{
	cli();
	uint8_t data = UDR0;
	Device.UserInputHandler.EnqueueUsartInput(data);
	sei();
}

void Usart0Init(uint32_t baud)
{
	uint16_t ubrr = F_CPU / 16 / baud - 1;
	UBRR0 = ubrr;									// Set baud rate
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);	// Enable receiver and transmitter; enable receive interrupt
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);				// Set frame format: 8data, 1stop bit
}

void Usart0Transmit(uint8_t data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );				// Wait for empty transmit buffer
	UDR0 = data;									// Put data into buffer, sends the data
}

void Usart0TransmitStr(const char* data)
{
	for (unsigned int i = 0; i < strlen(data); i++)
	{
		Usart0Transmit(data[i]);					// Transmit each character as byte
	}
}

uint8_t Usart0Receive()
{
	while ( !(UCSR0A & (1<<RXC0)) );				// Wait for data to be received
	return UDR0;									// Get and return received data from buffer
}

void Usart0Flush()
{
	__attribute__((unused)) unsigned char dummy;	// The __attribute__((unused)) is used to mark the dummy variable as unused (to suppress a compiler warning)
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

void Usart0ChangeBaudRate(uint32_t baud)
{
	while ( !( UCSR0A & (1<<UDRE0)) );				// Wait for empty transmit buffer (until all pending bytes are transmitted)
	UCSR0B &= ~(1<<RXEN0);							// Disable receiver
	UCSR0B &= ~(1<<TXEN0);							// Disable transmitter
	UCSR0B &= ~(1<<RXCIE0);							// Disable receive interrupt
	
	Usart0Init(baud);								// Reinitialize the USART0 interface
}