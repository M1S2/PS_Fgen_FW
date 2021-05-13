/*
 * Encoder.h
 *
 * Created: 07.09.2020 19:17:09
 *  Author: V17
 * see: https://www.leniwiec.org/en/2014/04/28/rotary-shaft-encoder-how-to-connect-it-and-handle-it-with-avr-atmega8-16-32-168-328/
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_

#include <avr/interrupt.h>
#include "../Pins/Pins.h"

void Encoder_Init();
bool Encoder_IsButtonPressed();

#endif /* ENCODER_H_ */