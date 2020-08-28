/*
 * PowerSupplySigGen.cpp
 *
 * Created: 14.08.2020 19:23:21
 * Author : V17
 */ 

#include <avr/io.h>
#include "Spi/spi.h"
#include "Pins/Pins.h"
#include "GLCD/VLGEM1277.h"

#include <avr/interrupt.h>

int main(void)
{
	cli();
	Pins_Init();
	SPI_Init();
	lcd_init();	
	sei();
	
	lcd_clear();

	// test
	lcd_clear();
	for (uint8_t y=0;y<8;y++) 
	{
		for (uint8_t i=0;i<40;i++) 
		{
			lcd_char(1+y%3,i,y,0x20+19+i-y);
		}
	}
	
		
    while (1) 
    {

    }
}

