/*
 * USART.h
 *
 * Created: 28.10.2020 17:14:38
 *  Author: V17
 */ 


#ifndef USART_H_
#define USART_H_

#include "../Pins/Pins.h"
#include <string.h>

void Usart0Init(uint32_t baud);
void Usart0Transmit(uint8_t data);	
void Usart0TransmitStr(const char* data);
uint8_t Usart0Receive();
void Usart0Flush();
void Usart0ChangeBaudRate(uint32_t baud);


#endif /* USART_H_ */