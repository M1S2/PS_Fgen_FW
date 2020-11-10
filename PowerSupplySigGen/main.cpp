/*
 * PowerSupplySigGen.cpp
 *
 * Created: 14.08.2020 19:23:21
 * Author : V17
 */ 

/********************************************************
* ATMega 1284P
* avrdude -p m1284p -P usb -c usbasp 
* -U lfuse:w:0xFF:m (external Crystal 8-16 MHz)
* -U hfuse:w:0xD9:m (JTAG Interface Disable)
********************************************************/

#include <avr/io.h>
#include "Spi/spi.h"
#include "Pins/Pins.h"
#include "GLCD/u8g.h"
#include "Outputs/ADC_MCP492x.h"
#include "Outputs/PowerSupply.h"
#include "KeyPad/KeyPad.h"
#include "Encoder/Encoder.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "Global/DevSettings.h"
#include "Global/DevStatus.h"

#include "UserControls/UserControlBool.h"
#include "UserControls/UserControlEnum.h"
#include "UserControls/UserControlNumeric.h"
#include "UserInputHandler/CircularBuffer.h"
#include "UserInputHandler/UserInputHandler.h"

#include "Screens/ScreenManager.h"

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

//float num = 0;
//UserControlNumeric<float> ctrlNum (SCREEN_TAB_WIDTH + 75, 10, &num, "V", -3, -10000, 10000);


extern EncoderDirection_t EncoderDir;
DevSettings_t DevSettings;
DevStatus_t DevStatus;


ISR(TIMER1_COMPA_vect)
{
	Keys_t key = KeyPad_GetKeys();
	if(key != KEYNONE)
	{
		UserInputHandler.EnqueueKeyInput(key);
	}
	if(Encoder_IsButtonPressed())
	{
		UserInputHandler.EnqueueEncoderButtonInput();
	}
	
	//ctrlNum.KeyInput(key);
}

/* Initialize 16-bit Timer/Counter1 */
void InitUserTimer()
{
	TCCR1B = (1 << WGM12);					// Configure for CTC mode
	TCCR1B |= ((1 << CS10) | (1 << CS11));	// Prescaler 64
	TIMSK1 = (1 << OCIE1A);					// Enable Output Compare A Match Interrupt
	OCR1A = (F_CPU / 64 / 5);				// Set compare register A (5 Hz)
}

int main(void)
{
	cli();
	Pins_Init();
	SPI_Init();
	Encoder_Init();
	ADC_init();
	Usart0Init(9600);
	InitUserTimer();
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
	
	//ctrlNum.IsSelected = true;
	//ctrlNum.IsActive = true;
	DevSettings.TabIndex = 0;
	
	ScreenManager.SetU8GLib_Object(&u8g);
	
	for(;;)
	{		
		UserInputHandler.ProcessInputs();	
		
		DevStatus_t devStatusDraw = DevStatus;
		DevSettings_t devSettingsDraw = DevSettings;
		u8g_FirstPage(&u8g);
		do
		{
			ScreenManager.Draw(devStatusDraw, devSettingsDraw);
			
			//ctrlNum.Draw(&u8g);
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(100);
		
		bool encPb = Encoder_IsButtonPressed();
		/*if(encPb)
		{			
			DevSettings.PS_Output_Enabled = !DevSettings.PS_Output_Enabled;
		}*/
		
		if(EncoderDir == ENCCLOCKWISE)
		{			
			//DevSettings.PS_Voltage_mV += 500;
		}
		else if(EncoderDir == ENCCOUNTERCLOCKWISE)
		{				
			//DevSettings.PS_Voltage_mV -= 500;
		}
		
		//ctrlNum.EncoderInput(EncoderDir);
		
		if(DevSettings.PS_Voltage_mV < 0) { DevSettings.PS_Voltage_mV = 0; }
		else if(DevSettings.PS_Voltage_mV > 10000) { DevSettings.PS_Voltage_mV = 10000; }
		PS_Output_Set();
		
		EncoderDir = ENCNONE;
		
		//Keys_t key = KeyPad_GetKeys();
		//ctrlNum.KeyInput(key);
		
		//ctrlBool.KeyInput(key);
		//ctrlEnum.KeyInput(key);
		/*if(key == KEYRIGHT)
		{
			DevSettings.TabIndex++;
			DevSettings.TabIndex %= NUM_SCREENS;
		}
		else if(key == KEYLEFT)
		{
			if(DevSettings.TabIndex == 0) { DevSettings.TabIndex = NUM_SCREENS - 1; }
			else { DevSettings.TabIndex--; }
		}*/
	}
	
}