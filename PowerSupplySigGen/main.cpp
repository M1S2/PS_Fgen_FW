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
#include "ADC_MCP492x/ADC_MCP492x.h"
#include "Screens/screen_draw_tabs.h"
#include "Screens/screen_draw_TabPS.h"

#include <avr/interrupt.h>

u8g_t u8g;
char tabIndex = 0;

void draw(void)
{
	Screen_DrawTabs(&u8g, tabIndex);
	if(tabIndex == 0) { Screen_DrawTabPS(&u8g); }
}

int main(void)
{
	cli();
	Pins_Init();
	SPI_Init();
	MCP4921_init(1, 5);
	MCP4922_init(1, 5);
	sei();
	
	MCP4922_DisableLatching();
	MCP4921_Voltage_Set(1);
	MCP4922_Voltage_Set(2.5, 'A');
	MCP4922_Voltage_Set(5, 'B');
	
	u8g_InitSPI(&u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	
	for(;;)
	{
		u8g_FirstPage(&u8g);
		do
		{
			draw();
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(3000);
		//tabIndex++;
		tabIndex %= SCREEN_NUM_TABS;
	}
	
}