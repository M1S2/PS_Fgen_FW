/*
 * OnOffControls.cpp
 * Created: 29.08.2024 18:39:00
 * Author: Markus Scheich
 */ 

#include "OnOffControls.h"
#include "MCP23S08.h"
#include "../Device.h"

MCP23S08 expander;

void OnOffControls_Init()
{
	expander.begin();
	expander.pinModeIO(ONOFFLED_MCP23S08_PIN_PS, MCP23S08_PINMODE_OUTPUT);
	expander.pinModeIO(ONOFFLED_MCP23S08_PIN_PS_ERROR, MCP23S08_PINMODE_OUTPUT);
	expander.pinModeIO(ONOFFLED_MCP23S08_PIN_DDS1, MCP23S08_PINMODE_OUTPUT);
	expander.pinModeIO(ONOFFLED_MCP23S08_PIN_DDS2, MCP23S08_PINMODE_OUTPUT);
	OnOffControls_SetLEDState(ONOFFLED_PS, false);
	OnOffControls_SetLEDState(ONOFFLED_PS_ERROR, false);
	OnOffControls_SetLEDState(ONOFFLED_DDS1, false);
	OnOffControls_SetLEDState(ONOFFLED_DDS2, false);
	
	expander.pinModeIO(ONOFFBUTTON_MCP23S08_PIN_PS, MCP23S08_PINMODE_INPUT_PULLUP);
	expander.pinModeIO(ONOFFBUTTON_MCP23S08_PIN_DDS1, MCP23S08_PINMODE_INPUT_PULLUP);
	expander.pinModeIO(ONOFFBUTTON_MCP23S08_PIN_DDS2, MCP23S08_PINMODE_INPUT_PULLUP);
	expander.setInterruptControl(~ONOFFBUTTON_MCP23S08_PIN_MASK);		// compare the actual pin value against the previous value (not the DEFVAL register)
	expander.setINTPinPushPullActiveState(true);						// Interrupt drives the INT pin to low
	expander.setInterruptOnChange(ONOFFBUTTON_MCP23S08_PIN_MASK);		// enable interrupt on change for button pins
}

bool OnOffControls_IsButtonChanged()
{
	// The IO_EXP_INT pin is low if one of the on/off buttons changed it's state and is reset if the input state is read
	return BIT_IS_CLEARED(PIND, IO_EXP_INT);
}

OnOffButtons_t OnOffControls_GetButton()
{
	uint8_t inputStates = 0;
	if(OnOffControls_IsButtonChanged())
	{
		uint8_t flags = expander.getInterruptFlags();		// Reading the flags here seems to be necessary (otherwise the state of the PS button behaves not correct)
		inputStates = expander.getInterruptCaptures();
	}
	else
	{
		inputStates = expander.getInputStates();
	}

	#ifdef PS_SUBSYSTEM_ENABLED
		if((inputStates & (1 << ONOFFBUTTON_MCP23S08_PIN_PS)) == 0x0)		// button pins are low active
		{
			return ONOFFBUTTON_PS;
		}
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		if((inputStates & (1 << ONOFFBUTTON_MCP23S08_PIN_DDS1)) == 0x0)
		{
			return ONOFFBUTTON_DDS1;
		}
		if((inputStates & (1 << ONOFFBUTTON_MCP23S08_PIN_DDS2)) == 0x0)
		{
			return ONOFFBUTTON_DDS2;
		}
	#endif
	return ONOFFBUTTON_NONE;
}

void OnOffControls_DoButtonAction(OnOffButtons_t button)
{
	switch(button)
	{
		#ifdef PS_SUBSYSTEM_ENABLED
			case ONOFFBUTTON_PS:
			{
				// Toggle the enabled state of the PS channel
				Device.PsChannel.SetEnabled(!Device.PsChannel.GetEnabled());
				break;
			}
		#endif
		#ifdef DDS_SUBSYSTEM_ENABLED
			case ONOFFBUTTON_DDS1:
			{
				// Toggle the enabled state of the DDS1 channel
				Device.DdsChannel1.SetEnabled(!Device.DdsChannel1.GetEnabled());
				break;
			}
			case ONOFFBUTTON_DDS2:
			{
				// Toggle the enabled state of the DDS2 channel
				Device.DdsChannel2.SetEnabled(!Device.DdsChannel2.GetEnabled());
				break;
			}
		#endif
		default: break;
	}
}

void OnOffControls_SetLEDState(OnOffLEDs_t led, bool state)
{
	uint8_t led_pin = 255;		// initial value is a non-valid pin
	switch(led)
	{
		#ifdef PS_SUBSYSTEM_ENABLED
			case ONOFFLED_PS: led_pin = ONOFFLED_MCP23S08_PIN_PS; break;
			case ONOFFLED_PS_ERROR: led_pin = ONOFFLED_MCP23S08_PIN_PS_ERROR; break;
		#endif
		#ifdef DDS_SUBSYSTEM_ENABLED
			case ONOFFLED_DDS1: led_pin = ONOFFLED_MCP23S08_PIN_DDS1; break;
			case ONOFFLED_DDS2: led_pin = ONOFFLED_MCP23S08_PIN_DDS2; break;
		#endif
		default: break;
	}

	cli();	// Disable interrupt temporary to don't interrupt the command to the IO expander
	expander.digitalWriteIO(led_pin, state);
	sei();
}

void OnOffControls_UpdatePSLeds()
{
#ifdef PS_SUBSYSTEM_ENABLED
	if(!Device.PsChannel.GetEnabled() && !Device.PsChannel.IsProtectionActive())
	{
		OnOffControls_SetLEDState(ONOFFLED_PS, false);
		OnOffControls_SetLEDState(ONOFFLED_PS_ERROR, false);
	}
	else if(Device.PsChannel.GetEnabled() && !Device.PsChannel.IsProtectionActive())
	{
		OnOffControls_SetLEDState(ONOFFLED_PS, true);
		OnOffControls_SetLEDState(ONOFFLED_PS_ERROR, false);
	}
	else if(!Device.PsChannel.GetEnabled() && Device.PsChannel.IsProtectionActive())
	{
		OnOffControls_SetLEDState(ONOFFLED_PS, false);
		OnOffControls_SetLEDState(ONOFFLED_PS_ERROR, true);
	}
	else if(Device.PsChannel.GetEnabled() && Device.PsChannel.IsProtectionActive())
	{
		OnOffControls_SetLEDState(ONOFFLED_PS, false);
		OnOffControls_SetLEDState(ONOFFLED_PS_ERROR, true);
	}
#endif
}