/**
 * @file	NumericIndicator.h
 * @date	22.03.2021 19:08:37
 * @author	Markus Scheich
 * @brief	Containing a class for a numeric indicator that is only showing a numeric variable value.
 */ 

#ifndef NUMERICINDICATOR_H_
#define NUMERICINDICATOR_H_

#include "../Core/UIElement.h"

/**
 * Class for a numeric indicator that is only showing a numeric variable value.
 * @tparam T Type of numeric variable handled by this indicator. This can be e.g. float or int.
 * @tparam stringBufferLength Length for the internally used _stringDrawBuffer
 */
template <class T, int stringBufferLength = 9>
class NumericIndicator : public UIElement
{
	private:
		/** Calculate the _displayValue and the _unitPrefix from the _valueDraw. */
		void calculateDisplayValue();
		
		/**
		 * Calculate the number of non-fractional digits of the given number.
		 * @see https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
		 */
		int numNonFractionalDigits(T number);
		
		T _lastValueDraw;								/**< Last drawn numeric value. Only if the _valueDraw differs from this value, the _stringDrawBuffer is recalculated. */
		bool _firstDraw;								/**< Used to track if it's the first draw after the construction of the indicator. */
		const char* _unitPrefix;						/**< Current display prefix character ("m", "k", "M") */
		char _stringDrawBuffer[stringBufferLength];		/**< Buffer holding the string that is drawn to the screen. This is only recalculated on the firstPage and if the value has changed. `char stringBufferLen = _numDigits + 1 + strlen(_unitPrefix) + strlen(_baseUnit) + 1;` */
		
	protected:
		const char* _baseUnit;							/**< Base unit that is appended to the calculated prefix. E.g. "V" for voltage values. To use the "%" sign as unit, you have to use "%%" as string. */
		T* _valuePointer;								/**< Pointer to the numeric variable that is shown by this indicator. */
		T _valueDraw;									/**< This variable is updated from the _valuePointer on each draw of the first page. */
		T _maxValue;									/**< Maximum value that can be shown by this numeric indicator. It is used to determine the number of non-fractional digits. */
		unsigned char _numFractionalDigits;				/**< Number of fractional digits that are shown by this indicator. E.g. 1.234 V has 3 fractional digits. */
		unsigned char _numDigits;						/**< Number of digits calculated from the maxValue (_numFractionalDigits + numNonFractionalDigits). */

		float _displayValue;							/**< Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz. */
		signed char _unitPrefixPower;					/**< Current display prefix power (m = -3, k = 3, M = 6) */

	public:
			
		/**
		 * Constructor of the NumericIndicator.
		 * @param locX X Location of the upper left corner of the NumericIndicator 
		 * @param locY Y Location of the upper left corner of the NumericIndicator
		 * @param valuePointer Pointer to the numeric variable that is shown by this indicator.
		 * @param baseUnit Base unit that is appended to the calculated prefix. E.g. "V" for voltage values. To use the "%" sign as unit, you have to use "%%" as string.
		 * @param maxValue Maximum value that can be shown by this numeric indicator. It is used to determine the number of non-fractional digits.
		 * @param numFractionalDigits Number of fractional digits that are shown by this indicator. E.g. 1.234 V has 3 fractional digits.
		 */
		NumericIndicator(unsigned char locX, unsigned char locY, T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits);
		
		/**
		 * Method used for drawing of the NumericIndicator.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* NUMERICINDICATOR_H_ */