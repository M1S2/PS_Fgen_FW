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
#include "Screens/screen_draw_TabDMM.h"
#include "Screens/screen_draw_TabATX.h"
#include "KeyPad/KeyPad.h"
#include "Encoder/Encoder.h"
#include "ADC/ADC.h"
#include "Global/DevSettings.h"
#include "Global/DevStatus.h"

#include <avr/interrupt.h>

u8g_t u8g;

extern EncoderDirection_t EncoderDir;
DevSettings_t DevSettings;
DevStatus_t DevStatus;

void draw(DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw)
{
	Screen_DrawTabs(&u8g, devSettingsDraw.TabIndex);
	switch(devSettingsDraw.TabIndex)
	{
		case 0: Screen_DrawTabPS(&u8g, devStatusDraw, devSettingsDraw); break;
		case 3: Screen_DrawTabDMM(&u8g, devStatusDraw); break;
		case 4: Screen_DrawTabATX(&u8g, devStatusDraw); break;
		default: break;
	}
	
	/*u8g_SetFont(&u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(&u8g);
	
	switch(EncoderDir)
	{
		case ENCCLOCKWISE: u8g_DrawStr(&u8g, 10, 10, "+"); break;
		case ENCCOUNTERCLOCKWISE: u8g_DrawStr(&u8g, 10, 10, "-"); break;
		default: u8g_DrawStr(&u8g, 10, 10, "ENC"); break;
	}*/
	
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
	Encoder_Init();
	ADC_init();
	sei();
	
	ADC_startConversion();
	MCP4922_DisableLatching();
	MCP4921_Voltage_Set(1000);
	MCP4922_Voltage_Set(2500, 'A');
	MCP4922_Voltage_Set(5000, 'B');
	
	u8g_InitSPI(&u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	
	DevSettings.PS_Voltage_mV = 5000;
	DevSettings.PS_Output_Enabled = 0;
	PS_Output_Set();
	
	for(;;)
	{
		DevStatus_t devStatusDraw = DevStatus;
		DevSettings_t devSettingsDraw = DevSettings;
		u8g_FirstPage(&u8g);
		do
		{
			draw(devStatusDraw, devSettingsDraw);
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(150);
		
		bool encPb = Encoder_IsButtonPressed();
		if(encPb)
		{
			DevSettings.PS_Output_Enabled = !DevSettings.PS_Output_Enabled;
		}
		
		if(EncoderDir == ENCCLOCKWISE)
		{
			DevSettings.PS_Voltage_mV += 500;
		}
		else if(EncoderDir == ENCCOUNTERCLOCKWISE)
		{
			DevSettings.PS_Voltage_mV -= 500;
		}
		
		if(DevSettings.PS_Voltage_mV < 0) { DevSettings.PS_Voltage_mV = 0; }
		else if(DevSettings.PS_Voltage_mV > 10000) { DevSettings.PS_Voltage_mV = 10000; }
		PS_Output_Set();
		
		EncoderDir = ENCNONE;
		
		Keys_t key = KeyPad_GetKeys();
		if(key == KEYRIGHT)
		{
			DevSettings.TabIndex++;
			DevSettings.TabIndex %= SCREEN_NUM_TABS;
		}
		else if(key == KEYLEFT)
		{
			if(DevSettings.TabIndex == 0) { DevSettings.TabIndex = SCREEN_NUM_TABS - 1; }
			else { DevSettings.TabIndex--; }
		}
	}
	
}