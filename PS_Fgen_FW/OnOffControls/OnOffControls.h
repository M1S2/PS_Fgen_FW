/**
 * @file	OnOffControls.h
 * @date	29.08.2024 18:39:00
 * @author	Markus Scheich
 * @brief	Containing methods to handle on/off buttons
 */ 

#ifndef ONOFFCONTROLS_H_
#define ONOFFCONTROLS_H_

#include "../Pins/Pins.h"

// button pins on the MCP23S08 IO expander
#define ONOFFBUTTON_MCP23S08_PIN_PS		5		/**< Pin on the IO expander for the PS button */
#define ONOFFBUTTON_MCP23S08_PIN_DDS1	3		/**< Pin on the IO expander for the DDS1 button */
#define ONOFFBUTTON_MCP23S08_PIN_DDS2	1		/**< Pin on the IO expander for the DDS2 button */
#define ONOFFBUTTON_MCP23S08_PIN_MASK	((1<<ONOFFBUTTON_MCP23S08_PIN_PS) | (1<<ONOFFBUTTON_MCP23S08_PIN_DDS1) | (1<<ONOFFBUTTON_MCP23S08_PIN_DDS2))				/**< Mask for all button pins on the IO expander */

// LED pins on the MCP23S08 IO expander
#define ONOFFLED_MCP23S08_PIN_PS		4		/**< Pin on the IO expander for the PS LED */
#define ONOFFLED_MCP23S08_PIN_PS_ERROR	6		/**< Pin on the IO expander for the PS error LED */
#define ONOFFLED_MCP23S08_PIN_DDS1		2		/**< Pin on the IO expander for the DDS1 LED */
#define ONOFFLED_MCP23S08_PIN_DDS2		0		/**< Pin on the IO expander for the DDS2 LED */

/**
 * Enumeration with all available on/off buttons.
 */
typedef enum OnOffButtons
{
	ONOFFBUTTON_NONE,	/**< no on/off button is pressed*/
	ONOFFBUTTON_PS,		/**< on/off button for the PS channel */
	ONOFFBUTTON_DDS1,	/**< on/off button for the DDS1 channel */
	ONOFFBUTTON_DDS2,	/**< on/off button for the DDS2 channel */
}OnOffButtons_t;

/**
 * Enumeration with all available on/off LEDs.
 */
typedef enum OnOffLEDs
{
	ONOFFLED_PS,		/**< on/off LED for the PS channel */
	ONOFFLED_PS_ERROR,	/**< on/off LED for the PS channel error */
	ONOFFLED_DDS1,		/**< on/off LED for the DDS1 channel */
	ONOFFLED_DDS2,		/**< on/off LED for the DDS2 channel */
}OnOffLEDs_t;

/**
 * Initialize the IO expander used to access the on/off controls.
 */	
void OnOffControls_Init();

/**
 * Check if the state of any on/off button has changed (was pressed or was released).
 * @return true, if any button state changed; otherwise false.
 */	
bool OnOffControls_IsButtonChanged();

/**
 * Get the last pressed on/off button.
 * Only one button is returned even if multiple buttons were pressed.
 * @return button that was pressed or ONOFFBUTTON_NONE if no button was pressed
 */	
OnOffButtons_t OnOffControls_GetButton();

/**
 * Do some action depending on the pressed on/off button.
 * @param button on/off button that should be handled.
 */	
void OnOffControls_DoButtonAction(OnOffButtons_t button);

/**
 * Set the state of the on/off LED.
 * @param led on/off LED that should be set.
 * @param state when true, the LED is turned on; if false, the LED is turned off
 */	
void OnOffControls_SetLEDState(OnOffLEDs_t led, bool state);


/**
 * Update the PS and PS error LEDs depending on the output enabled and protection states of the PS channel.
 * Output Enabled	| Protection active	| LED PS	| LED PS error
 * ---------------- | ----------------- |---------- | -------------
 * false			| false				| off		| off
 * true				| false				| on		| off
 * false			| true				| off		| on
 * true				| true				| off		| on
 */
void OnOffControls_UpdatePSLeds();

#endif /* ONOFFCONTROLS_H_ */