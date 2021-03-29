/*
 * NumericControl.h
 *
 * Created: 25.03.2021 20:52:38
 *  Author: V17
 */ 


#ifndef NUMERICCONTROL_H_
#define NUMERICCONTROL_H_

#include "NumericIndicator.h"

template <class T>
class NumericControl : public NumericIndicator<T>
{
	private:
		signed char _currentDigitPosition;
		T _minValue;

		T coerceValue(T value);
		unsigned char extractDigit(float number, int8_t position);
		
		void* _controlContext;							// Context pointer that is returned with the _onValueChanged function pointer
		void(*_onValueChanged)(void* controlContext);	// Function pointer for _onValueChanged event

	public:
		bool IsEditMode;
	
		NumericControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;

		bool KeyKilo();
		bool KeyMilli();
		bool KeyMinus();
		bool KeyComma();			// Is used as x1 key
		bool KeyNumeric(Keys_t key);
		bool ValueUp();
		bool ValueDown();
		bool CursorLeft();
		bool CursorRight();
		void ToggleEditMode();
};

#endif /* NUMERICCONTROL_H_ */