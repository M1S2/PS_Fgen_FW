/*
 * NumericIndicator.h
 *
 * Created: 22.03.2021 19:08:37
 *  Author: V17
 */ 


#ifndef NUMERICINDICATOR_H_
#define NUMERICINDICATOR_H_

#include "UIElement.h"

template <class T>
class NumericIndicator : public UIElement
{
	private:
		void calculateDisplayValue();
		int numNonFractionalDigits(T number);
		T _lastValueDraw;
		const char* _unitPrefix;		// Current display prefix character (m, k, M)

	protected:
		const char* _baseUnit;
		T* _valuePointer;
		T _valueDraw;
		T _maxValue;
		unsigned char _numFractionalDigits;
		unsigned char _numDigits;

		float _displayValue;			// Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz.
		signed char _unitPrefixPower;			// Current display prefix power (m = -3, k = 3, M = 6)

	public:
		NumericIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* NUMERICINDICATOR_H_ */