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
#include "Outputs/ADC_MCP492x.h"
#include "Outputs/PowerSupply.h"
#include "Screens/screen_draw_tabs.h"
#include "Screens/screen_draw_TabPS.h"
#include "Screens/screen_draw_TabDMM.h"
#include "Screens/screen_draw_TabATX.h"
#include "KeyPad/KeyPad.h"
#include "Encoder/Encoder.h"
#include "ADC/ADC.h"
#include "Global/DevSettings.h"
#include "Global/DevStatus.h"

#include "UserControls/UserControlBool.h"

#include <avr/interrupt.h>

u8g_t u8g;
UserControlBool ctrlBool (SCREEN_TAB_WIDTH + 10, 10, "BoolCtrl", &DevSettings.PS_Output_Enabled);

extern EncoderDirection_t EncoderDir;
DevSettings_t DevSettings;
DevStatus_t DevStatus;

void draw(DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw)
{
	Screen_DrawTabs(&u8g, devSettingsDraw.TabIndex);
	switch(devSettingsDraw.TabIndex)
	{
		case 0: Screen_DrawTabPS(&u8g, devStatusDraw, devSettingsDraw); break;
		case 1: ctrlBool.Draw(&u8g); break;
		case 3: Screen_DrawTabDMM(&u8g, devStatusDraw); break;
		case 4: Screen_DrawTabATX(&u8g, devStatusDraw); break;
		default: break;
	}
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
	DevSettings.PS_Load_Impedance = 1000000;	//47;
	DevSettings.PS_Output_Enabled = 0;
	PS_Output_Set();
	
	ctrlBool.IsSelected = true;
	ctrlBool.IsActive = true;
	
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
		ctrlBool.EncoderInput(EncoderDir);
		
		if(DevSettings.PS_Voltage_mV < 0) { DevSettings.PS_Voltage_mV = 0; }
		else if(DevSettings.PS_Voltage_mV > 10000) { DevSettings.PS_Voltage_mV = 10000; }
		PS_Output_Set();
		
		EncoderDir = ENCNONE;
		
		Keys_t key = KeyPad_GetKeys();
		ctrlBool.KeyInput(key);
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