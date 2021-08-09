/**
 * @file	EnumControl.h
 * @date	15.03.2021 19:19:08
 * @author	Markus Scheich
 * @brief	Containing a class for a enum control that is showing a enumeration variable value and offers the possibility to edit the value.
 */ 

#ifndef ENUMCONTROL_H_
#define ENUMCONTROL_H_

#include "../Indicators/EnumIndicator.h"

/**
 * Class for a enum control that is showing a enumeration variable value and offers the possibility to edit the value.
 * @tparam T Type of enumeration handled by this control
 */
template <class T>
class EnumControl : public EnumIndicator<T>
{	
	private:
		void* _controlContext;							/**< Context pointer that is returned with the _onValueChanged function pointer */
		void(*_onValueChanged)(void* controlContext);	/**< Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed. */
		
	public:
		bool IsEditMode;								/**< Is the control in edit mode? All keys are only supported in edit mode. */
	
		/**
		 * Constructor of the EnumControl.
		 * @param locX X Location of the upper left corner of the EnumControl 
		 * @param locY Y Location of the upper left corner of the EnumControl
		 * @param valuePointer Pointer to the enumeration variable that is shown and manipulated by this control.
		 * @param enumNames Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values.
		 * @param numEnumValues Number of enumeration values. This number must match the real number of enum values in T.
		 * @param controlContext Context pointer that is returned with the _onValueChanged function pointer
		 * @param onValueChanged Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed.
		 */
		EnumControl(unsigned char locX, unsigned char locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);

		/**
		 * Method used for drawing of the EnumControl.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
			
		/**
		 * Set the value of the controlled enumeration variable to the previous enum value.
		 * If the current value is the first one, the control selects the last enum value to implement a circular behaviour.
		 */
		bool PreviousValue();
		
		/**
		 * Set the value of the controlled enumeration variable to the next enum value.
		 * If the current value is the last one, the control selects the first enum value to implement a circular behaviour.
		 */
		bool NextValue();
		
		/**
		 * Toggle the control between display and edit mode.
		 * Some keys are only supported in edit mode.
		 */
		void ToggleEditMode();
};

#endif /* ENUMCONTROL_H_ */