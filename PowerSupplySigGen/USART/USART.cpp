/*
 * USART.cpp
 *
 * Created: 28.10.2020 17:14:47
 *  Author: V17
 */ 

#include "USART.h"
#include "../UserInputHandler/UserInputHandler.h"

ISR (USART0_RX_vect)
{
	uint8_t data = UDR0;
	UserInputHandler.EnqueueUsartInput(data);
}

void Usart0Init(uint16_t baud)
{
	uint16_t ubrr = F_CPU / 16 / baud - 1;
	
	/* Set baud rate */
	UBRR0 = ubrr;
	/* Enable receiver and transmitter; enable receive interrupt */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void Usart0Transmit(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void Usart0TransmitStr(const char* data)
{
	for (unsigned int i = 0; i < strlen(data); i++)
	{
		Usart0Transmit(data[i]);
	}
}

uint8_t Usart0Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void Usart0Flush()
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}