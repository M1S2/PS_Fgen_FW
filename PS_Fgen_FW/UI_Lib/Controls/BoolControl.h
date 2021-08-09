/**
 * @file	BoolControl.h
 * @date	15.03.2021 19:12:14
 * @author	Markus Scheich
 * @brief	Containing a class for a boolean control that is showing a boolean variable value and offers the possibility to toggle the value.
 */ 

#ifndef BOOLCONTROL_H_
#define BOOLCONTROL_H_

#include "../Indicators/BoolIndicator.h"

/**
 * Class for a boolean control that is showing a boolean variable value and offers the possibility to toggle the value.
 */
class BoolControl : public BoolIndicator
{	
	private:
		void* _controlContext;							/**< Context pointer that is returned with the _onValueChanged function pointer */
		void(*_onValueChanged)(void* controlContext);	/**< Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed. */
	
	public:
	
		/**
		 * Constructor of the BoolControl.
		 * @param locX X Location of the upper left corner of the BoolControl 
		 * @param locY Y Location of the upper left corner of the BoolControl
		 * @param valuePointer Pointer to the boolean variable that is shown and manipulated by this control.
		 * @param controlContext Context pointer that is returned with the _onValueChanged function pointer
		 * @param onValueChanged Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed.
		 */
		BoolControl(unsigned char locX, unsigned char locY, bool* valuePointer, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);
		
		/**
		 * Method used for drawing of the BoolControl.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Toggle the value of the boolean variable referenced by the valuePointer.
		 */
		void ToggleValue();
};

#endif /* BOOLCONTROL_H_ */