/**
 * @file	Encoder.h
 * @date	07.09.2020 19:17:09
 * @author	Markus Scheich
 * @brief	Containing functions to handle the rotary encoder.
 * @see		https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/
 * @see		not used anymore: https://www.leniwiec.org/en/2014/04/28/rotary-shaft-encoder-how-to-connect-it-and-handle-it-with-avr-atmega8-16-32-168-328/
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_

#include <avr/interrupt.h>
#include "../Pins/Pins.h"

#include "../UI_Lib/Core/Keys.h"

/**
 * Initialize all registers for the pin interrupts used for encoder handling.
 */
void Encoder_Init();

/**
 * Check if the encoder button is pressed.
 * This checks if the ENC_PB pin is pulled low.
 * @return true if the button is pressed; otherwise false
 */
bool Encoder_IsButtonPressed();

/**
 * Get the direction in which the encoder is rotating (and if it is even rotating).
 * This should be cyclic (e.g. from within a timer loop).
 * @return KEYUP for clockwise rotation; KEYDOWN for counter-clockwise rotation; KEYNONE if not rotating
 */
Keys_t Encoder_GetDirection();

#endif /* ENCODER_H_ */