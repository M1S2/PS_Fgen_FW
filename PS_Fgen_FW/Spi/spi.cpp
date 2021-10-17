/*
 * spi.cpp
 * Created: 14.08.2020 19:24:05
 * Author: Markus Scheich
 * Source: https://github.com/0xD34D/ATmega/blob/master/spi.c
 */ 

#include "spi.h"

void SPI_Init()
{
	SPCR = (1 << SPE) | (1 << MSTR);		// SPI enabled, MSB first (DORD=0), master mode, F_osc/2
	SPSR = 1;								// set double SPI speed for F_osc/2
}