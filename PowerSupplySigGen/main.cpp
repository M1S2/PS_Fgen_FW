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
#include "UserControls/UserControlEnum.h"
#include "UserControls/UserControlNumeric.h"

#include <avr/interrupt.h>

u8g_t u8g;

typedef enum SignalForms
{
	SINE,
	RECTANGLE,
	TRIANGLE
} SignalForms_t;
const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE" };


/*UserControlBool ctrlBool (SCREEN_TAB_WIDTH + 10, 10, &DevSettings.PS_Output_Enabled);
SignalForms_t signalForm;
UserControlEnum<SignalForms_t> ctrlEnum (SCREEN_TAB_WIDTH + 10, 30, &signalForm, SignalFormsNames, 3);*/

float num = 0;
UserControlNumeric<float> ctrlNum (SCREEN_TAB_WIDTH + 75, 10, &num, "V", -3, -10000, 10000);


extern EncoderDirection_t EncoderDir;
DevSettings_t DevSettings;
DevStatus_t DevStatus;

void draw(DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw)
{
	Screen_DrawTabs(&u8g, devSettingsDraw.TabIndex);
	switch(devSettingsDraw.TabIndex)
	{
		case 0: Screen_DrawTabPS(&u8g, devStatusDraw, devSettingsDraw); break;
		case 1: /*ctrlBool.Draw(&u8g); ctrlEnum.Draw(&u8g);*/ ctrlNum.Draw(&u8g); break;
		case 3: /*Screen_DrawTabDMM(&u8g, devStatusDraw);*/ break;
		case 4: /*Screen_DrawTabATX(&u8g, devStatusDraw);*/ break;
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
	
	uint8_t selectedUserControlIndex = 0;
	//ctrlBool.IsSelected = true;
	//ctrlBool.IsActive = false;
	//ctrlEnum.IsSelected = false;
	//ctrlEnum.IsActive = false;
	ctrlNum.IsSelected = true;
	ctrlNum.IsActive = true;
	DevSettings.TabIndex = 1;
	
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
		/*if(encPb)
		{
			if(selectedUserControlIndex == 0)
			{
				ctrlBool.IsActive = !ctrlBool.IsActive;
			}
			if(selectedUserControlIndex == 1)
			{
				ctrlEnum.IsActive = !ctrlEnum.IsActive;
			}
			
			//DevSettings.PS_Output_Enabled = !DevSettings.PS_Output_Enabled;
		}*/
		
		if(EncoderDir == ENCCLOCKWISE)
		{
			selectedUserControlIndex++;
			if(selectedUserControlIndex > 1) { selectedUserControlIndex = 0; }
			
			//DevSettings.PS_Voltage_mV += 500;
		}
		else if(EncoderDir == ENCCOUNTERCLOCKWISE)
		{
			selectedUserControlIndex--;
			if(selectedUserControlIndex < 0) { selectedUserControlIndex = 1; }
				
			//DevSettings.PS_Voltage_mV -= 500;
		}
		
		/*if(selectedUserControlIndex == 0)
		{
			ctrlBool.IsSelected = true;
			ctrlEnum.IsSelected = false;
		}
		if(selectedUserControlIndex == 1)
		{
			ctrlBool.IsSelected = false;
			ctrlEnum.IsSelected = true;			
		}*/
		//ctrlBool.EncoderInput(EncoderDir);
		//ctrlEnum.EncoderInput(EncoderDir);
		ctrlNum.EncoderInput(EncoderDir);
		
		if(DevSettings.PS_Voltage_mV < 0) { DevSettings.PS_Voltage_mV = 0; }
		else if(DevSettings.PS_Voltage_mV > 10000) { DevSettings.PS_Voltage_mV = 10000; }
		PS_Output_Set();
		
		EncoderDir = ENCNONE;
		
		Keys_t key = KeyPad_GetKeys();
		//ctrlBool.KeyInput(key);
		//ctrlEnum.KeyInput(key);
		ctrlNum.KeyInput(key);
		/*if(key == KEYRIGHT)
		{
			DevSettings.TabIndex++;
			DevSettings.TabIndex %= SCREEN_NUM_TABS;
		}
		else if(key == KEYLEFT)
		{
			if(DevSettings.TabIndex == 0) { DevSettings.TabIndex = SCREEN_NUM_TABS - 1; }
			else { DevSettings.TabIndex--; }
		}*/
	}
	
}