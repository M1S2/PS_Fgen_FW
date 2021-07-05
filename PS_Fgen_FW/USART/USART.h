/**
 * @file	USART.h
 * @date	28.10.2020 17:14:38
 * @author	Markus Scheich
 * @brief	Containing defines and functions for basic USART initialization and handling.
 */ 

#ifndef USART_H_
#define USART_H_

#include "../Pins/Pins.h"
#include <string.h>

/**
 * Initialize the USART0 interface.
 * Set the baud rate, enable the receiver and transmitter, enable the receive interrupt and set the frame format (8data, 1stop bit)
 * @param baud Baud rate that should be used for the USART0 interface (e.g. 9600)
 */
void Usart0Init(uint32_t baud);

/**
 * Send one data byte with the USART0 interface.
 * @param data Data byte to send.
 */
void Usart0Transmit(uint8_t data);

/**
 * Transmit each character (byte) of the string with the USART0 interface.
 * @param data String to send.
 */	
void Usart0TransmitStr(const char* data);

/**
 * Receive one byte with the USART0 interface.
 * @return Received data byte.
 */
uint8_t Usart0Receive();

/**
 * Flush the USART0 interface buffer.
 */
void Usart0Flush();

/**
 * Change the baud rate of the USART0 interface.
 * @param baud New baud rate of the USART0 interface.
 */
void Usart0ChangeBaudRate(uint32_t baud);

#endif /* USART_H_ */