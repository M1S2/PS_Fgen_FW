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
#include "KeyPad/KeyPad.h"
#include "Encoder/Encoder.h"

#include <avr/interrupt.h>

u8g_t u8g;
char tabIndex = 0;

extern EncoderDirection_t EncoderDir;

void draw(void)
{
	/*u8g_SetFont(&u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(&u8g);
	
	switch(EncoderDir)
	{
		case ENCCLOCKWISE: u8g_DrawStr(&u8g, 10, 10, "+"); break;
		case ENCCOUNTERCLOCKWISE: u8g_DrawStr(&u8g, 10, 10, "-"); break;
		default: u8g_DrawStr(&u8g, 10, 10, "ENC"); break;
	}*/

	Screen_DrawTabs(&u8g, tabIndex);
	if(tabIndex == 0) { Screen_DrawTabPS(&u8g); }
	
	/*
	u8g_SetFont(&u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(&u8g);
	Keys_t key = KeyPad_GetKeys(&u8g);
	switch(key)
	{
		case KEY0: u8g_DrawStr(&u8g, 10, 10, "0"); break;
		case KEY1: u8g_DrawStr(&u8g, 10, 10, "1"); break;
		case KEY2: u8g_DrawStr(&u8g, 10, 10, "2"); break;
		case KEY3: u8g_DrawStr(&u8g, 10, 10, "3"); break;
		case KEY4: u8g_DrawStr(&u8g, 10, 10, "4"); break;
		case KEY5: u8g_DrawStr(&u8g, 10, 10, "5"); break;
		case KEY6: u8g_DrawStr(&u8g, 10, 10, "6"); break;
		case KEY7: u8g_DrawStr(&u8g, 10, 10, "7"); break;
		case KEY8: u8g_DrawStr(&u8g, 10, 10, "8"); break;
		case KEY9: u8g_DrawStr(&u8g, 10, 10, "9"); break;
		case KEYKILO: u8g_DrawStr(&u8g, 10, 10, "k"); break;
		case KEYMILLI: u8g_DrawStr(&u8g, 10, 10, "m"); break;
		case KEYMINUS: u8g_DrawStr(&u8g, 10, 10, "-"); break;
		case KEYCOMMA: u8g_DrawStr(&u8g, 10, 10, "."); break;
		case KEYLEFT: u8g_DrawStr(&u8g, 10, 10, "<"); break;
		case KEYRIGHT: u8g_DrawStr(&u8g, 10, 10, ">"); break;
		default: u8g_DrawStr(&u8g, 10, 10, "NONE"); break;
	}*/
}

int main(void)
{
	cli();
	Pins_Init();
	SPI_Init();
	MCP4921_init(1, 5);
	MCP4922_init(1, 5);
	Encoder_Init();
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
		u8g_Delay(250);
		
		bool encPb = Encoder_IsButtonPressed();
		Keys_t key = KeyPad_GetKeys();
		if(key == KEYRIGHT || EncoderDir == ENCCLOCKWISE || encPb)
		{
			tabIndex++;
			tabIndex %= SCREEN_NUM_TABS;
		}
		else if(key == KEYLEFT || EncoderDir == ENCCOUNTERCLOCKWISE)
		{
			if(tabIndex == 0) { tabIndex = SCREEN_NUM_TABS - 1; }
			else { tabIndex--; }
		}
		EncoderDir = ENCNONE;
	}
	
}