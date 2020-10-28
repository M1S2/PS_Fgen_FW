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

void Usart0Init(uint16_t baud);
void Usart0Transmit(uint8_t data);
void Usart0TransmitStr(char* data);
uint8_t Usart0Receive();
void Usart0Flush();


#endif /* USART_H_ */