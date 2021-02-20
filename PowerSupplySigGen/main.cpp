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
* -U hfuse:w:0xD1:m (JTAG Interface Disable, Preserve EEPROM memory through the Chip Erase cycle)
* -U efuse:w:0xFC:m (Brown-out detection level at VCC=4.3 V)
********************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Spi/spi.h"
#include "Pins/Pins.h"
#include "GLCD/u8g.h"
#include "Channels/DAC_MCP492x.h"
#include "Channels/DDS_Channel.h"
#include "KeyPad/KeyPad.h"
#include "Encoder/Encoder.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "SCPI/SCPI_Device.h"
#include "Screens/ScreenManager.h"
#include "Configuration.h"

#include "Device.h"

#include <stdio.h>

bool redraw_screen;

ISR(TIMER1_COMPA_vect)
{
	redraw_screen = true;
	Device.UserTimerTickCounter++;
	Keys_t key = KeyPad_GetKeys();
	if(key != KEYNONE)
	{
		Device.UserInputHandler.EnqueueKeyInput(key);
	}
	if(Encoder_IsButtonPressed())
	{
		Device.UserInputHandler.EnqueueEncoderButtonInput();
	}
}

int main(void)
{
	Device.Init();
	
	for(;;)
	{		
		Device.UserInputHandler.ProcessInputs();
		
		if(redraw_screen)
		{
			Device.ScreenManager.DrawAll();
			redraw_screen = false;
		}
			
		#ifdef SPLASHSCREEN_ENABLED
			/* Hide splash screen after some time */
			if(Device.ScreenManager.IsSplashScreenShown && ((Device.UserTimerTickCounter * (1 / (float)USER_TIMER_TICK_FREQ)) >= SPLASHSCREEN_DELAY_SEC))
			{
				Device.ScreenManager.IsSplashScreenShown = false;
			}
		#endif
		
		if((Device.UserTimerTickCounter * (1 / (float)USER_TIMER_TICK_FREQ)) >= SETTINGS_AUTOSAVE_DELAY_SEC)
		{
			if(Device.DevSettingsNeedSaving)
			{
				Device.SaveSettings();
			}
			Device.UserTimerTickCounter = 0;
		}
	}
	
}