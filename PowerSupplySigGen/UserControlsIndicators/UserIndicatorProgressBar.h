/*
 * UserIndicatorProgressBar.h
 *
 * Created: 15.01.2021 19:02:40
 *  Author: V17
 */ 

#ifndef USERINDICATORPROGRESSBAR_H_
#define USERINDICATORPROGRESSBAR_H_

#include "UserIndicatorNumeric.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum ProgressBarOrigin
{
	PROGRESSBAR_ORIGIN_LEFT,
	PROGRESSBAR_ORIGIN_ZERO,
	PROGRESSBAR_ORIGIN_RIGHT
}ProgressBarOrigin_t;

template <class T>
class UserIndicatorProgressBar : public UserIndicatorNumeric<T>
{
private:
	T _minValue;
	T _maxValue;
	int _barWidth;
	int _barHeight;
	ProgressBarOrigin_t _origin;
	T _tickIncrement;

	int _originXCoord;
	
	int xCoordinateFromValue(T value)
	{
		return UserIndicatorNumeric<T>::_locX + 75 + (int)(((value - _minValue) / (float)(_maxValue - _minValue)) * (_barWidth - 1));
	}
	
public:
	UserIndicatorProgressBar(uint8_t locx, uint8_t locy, T* controlValue, const char* baseUnit, T minValue, T maxValue, int barWidth, int barHeight, ProgressBarOrigin_t origin, float tickIncrement) : UserIndicatorNumeric<T>(locx, locy, controlValue, baseUnit)
    {
		_minValue = minValue;
		_maxValue = maxValue;
		_barWidth = barWidth;
		_barHeight = barHeight;
		_origin = origin;
		_tickIncrement = tickIncrement;
		
		_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
	}
        		
    void Draw(u8g_t *u8g)
	{
		UserIndicatorNumeric<T>::Draw(u8g);

		// Draw outer border of progress bar
		u8g_DrawFrame(u8g, xCoordinateFromValue(_minValue), UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), _barWidth, _barHeight);

		u8g_SetFont(u8g, u8g_font_5x7r);
		char buffer[6];
		itoa(_maxValue, buffer, 10);
		u8g_DrawStr(u8g, xCoordinateFromValue(_maxValue) + 3, UserIndicatorNumeric<T>::_locY + 5, buffer);
		itoa(_minValue, buffer, 10);
		u8g_DrawStr(u8g, xCoordinateFromValue(_minValue) - 15, UserIndicatorNumeric<T>::_locY + 5, buffer);

		int valueXCoord = xCoordinateFromValue(*UserIndicatorNumeric<T>::_controlValue);
		u8g_DrawBox(u8g, MIN(valueXCoord, _originXCoord), UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), abs(valueXCoord - _originXCoord), _barHeight);
		
		for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
		{
			int xCoord = xCoordinateFromValue(xVal);
			int tickLength = (((int)xVal) % 10 == 0 ? 3 : (((int)xVal) % 5 == 0 ? 2 : 1));
			u8g_DrawVLine(u8g, xCoord, UserIndicatorNumeric<T>::_locY - tickLength, tickLength);
			//u8g_DrawVLine(u8g, xCoord, UserIndicatorNumeric<T>::_locY + _barHeight, tickLength);
		}
	}
};


#endif /* USERINDICATORPROGRESSBAR_H_ */