/*
 * PowerSupplySigGen.cpp
 *
 * Created: 14.08.2020 19:23:21
 * Author : V17
 */ 

#include <avr/io.h>
#include "Spi/spi.h"
#include "Pins/Pins.h"
#include "GLCD/u8g.h"

#include <avr/interrupt.h>

u8g_t u8g;

void draw(void)
{
	u8g_SetDefaultForegroundColor(&u8g);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 7, 15, "Hello World!");
	u8g_SetFont(&u8g, u8g_font_freedoomr10r);
	u8g_DrawStr(&u8g, 7, 40, "ABC");
	u8g_SetFont(&u8g, u8g_font_fub49n);
	u8g_DrawStr(&u8g, 70, 55, "0");
	u8g_SetDefaultMidColor(&u8g);
	u8g_DrawLine(&u8g, 5, 5, 235, 5);
	u8g_DrawLine(&u8g, 5, 5, 5, 60);
	u8g_DrawLine(&u8g, 235, 5, 235, 60);
	u8g_DrawLine(&u8g, 5, 60, 235, 60);
	u8g_DrawCircle(&u8g, 200, 33, 25, U8G_DRAW_ALL);
}

int main(void)
{
	cli();
	Pins_Init();
	SPI_Init();
	sei();
	
	u8g_InitSPI(&u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	
	for(;;)
	{
		u8g_FirstPage(&u8g);
		do
		{
			draw();
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(100);
	}
	
}