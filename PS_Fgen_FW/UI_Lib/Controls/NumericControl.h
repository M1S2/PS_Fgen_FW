/**
 * @file	NumericControl.h
 * @date	25.03.2021 20:52:38
 * @author	Markus Scheich
 * @brief	Containing a class for a numeric control that is showing a numeric variable value and offers the possibility to edit the value.
 */ 

#ifndef NUMERICCONTROL_H_
#define NUMERICCONTROL_H_

#include "../Indicators/NumericIndicator.h"

/**
 * Class for a numeric control that is showing a numeric variable value and offers the possibility to edit the value.
 * @tparam T Type of numeric variable handled by this control. This can be e.g. float or int.
 */
template <class T>
class NumericControl : public NumericIndicator<T>
{
	private:
		signed char _currentDigitPosition;				/**< Position of the currently controlled digit. Range from (-_numFractionalDigits) to (_numDigits - _numFractionalDigits - 1) */
		T _minValue;									/**< Minimum value that can be handled by this numeric control. */

		/**
		 * Limit the value to be between _minValue and _maxValue.
		 * @param value Value to coerce.
		 * @return Coerced value.
		 */
		T coerceValue(T value);
		
		/**
		 * Extract the digit at the requested position from the number.
		 * @param number Number from which to extract one digit.
		 * @param position position in the number from which to extract the digit. Position 0 is are 1er. Negative positions are fractionals. Positive positions are non-fractionals.
		 * @return Extracted digit between 0 and 9
		 * @see https://www.quora.com/How-can-you-mathematically-extract-a-single-digit-from-a-number
		 */
		unsigned char extractDigit(float number, int8_t position);
		
		void* _controlContext;							/**< Context pointer that is returned with the _onValueChanged function pointer */
		void(*_onValueChanged)(void* controlContext);	/**< Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed. */
		
	public:
		bool IsEditMode;								/**< Is the control in edit mode? All keys are only supported in edit mode. */
	
		/**
		 * Constructor of the NumericControl.
		 * @param locX X Location of the upper left corner of the NumericControl 
		 * @param locY Y Location of the upper left corner of the NumericControl
		 * @param valuePointer Pointer to the numeric variable that is shown and manipulated by this control.
		 * @param baseUnit Base unit that is appended to the calculated prefix. E.g. "V" for voltage values.
		 * @param minValue Minimum value that can be handled by this numeric control.
		 * @param maxValue Maximum value that can be shown by this numeric control. It is used to determine the number of non-fractional digits.
		 * @param numFractionalDigits Number of fractional digits that are shown by this control. E.g. 1.234 V has 3 fractional digits.
		 * @param controlContext Context pointer that is returned with the _onValueChanged function pointer
		 * @param onValueChanged Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed.
		 */
		NumericControl(unsigned char locX, unsigned char locY, T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);
		
		/**
		 * Method used for drawing of the NumericControl.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYLEFT, KEYRIGHT, KEYUP, KEYDOWN, KEYKILO, KEYMILLI, KEYX1, KEYMINUS, KEY0 .. KEY9
s		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;


		/**
		 * Kilo key is used to change the prefix to kilo.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyKilo();
		
		/**
		 * Milli key is used to change the prefix to milli.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyMilli();
		
		/**
		 * Minus key is used to toggle the sign of the controlled value.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyMinus();
		
		/**
		 * X1 key is used to change the prefix to none.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyX1();
		
		/**
		 * Numeric keys (KEY0 .. KEY9) change the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyNumeric(Keys_t key);
		
		/**
		 * Up key is used to increment the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool ValueUp();
		
		/**
		 * Down key is used to decrement the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool ValueDown();
		
		/**
		 * Left key is used to move the cursor position left.
		 * @return True if the key was handled, otherwise False.
		 */
		bool CursorLeft();

		/**
		 * Right key is used to move the cursor position right.
		 * @return True if the key was handled, otherwise False.
		 */
		bool CursorRight();
		
		/**
		 * Toggle the control between display and edit mode.
		 * Some keys are only supported in edit mode.
		 */
		void ToggleEditMode();
};

#endif /* NUMERICCONTROL_H_ */