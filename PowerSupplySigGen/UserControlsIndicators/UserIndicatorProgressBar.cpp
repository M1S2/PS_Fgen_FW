/*
 * UserIndicatorProgressBar.cpp
 *
 * Created: 28.01.2021 19:30:28
 *  Author: V17
 */ 

#include "UserIndicatorProgressBar.h"

template <class T>
int UserIndicatorProgressBar<T>::xCoordinateFromValue(T value)
{
	return UserIndicatorNumeric<T>::_locX + 75 + (int)(((value - _minValue) / (float)(_maxValue - _minValue)) * (_barWidth - 1));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
UserIndicatorProgressBar<T>::UserIndicatorProgressBar(uint8_t locx, uint8_t locy, T* controlValuePointer, const char* baseUnit, T minValue, T maxValue, int barWidth, int barHeight, ProgressBarOrigin_t origin, float tickIncrement) : UserIndicatorNumeric<T>(locx, locy, controlValuePointer, baseUnit)
{
	_minValue = minValue;
	_maxValue = maxValue;
	_barWidth = barWidth;
	_barHeight = barHeight;
	_origin = origin;
	_tickIncrement = tickIncrement;
	
	_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
}

template <class T>
void UserIndicatorProgressBar<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserIndicatorNumeric<T>::Draw(u8g, isFirstPage);

	// Draw outer border of progress bar
	u8g_DrawFrame(u8g, xCoordinateFromValue(_minValue), UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), _barWidth, _barHeight);

	u8g_SetFont(u8g, u8g_font_5x7r);
	char buffer[6];
	itoa(_maxValue, buffer, 10);
	u8g_DrawStr(u8g, xCoordinateFromValue(_maxValue) + 3, UserIndicatorNumeric<T>::_locY + 5, buffer);
	itoa(_minValue, buffer, 10);
	u8g_DrawStr(u8g, xCoordinateFromValue(_minValue) - 15, UserIndicatorNumeric<T>::_locY + 5, buffer);

	int valueXCoord = xCoordinateFromValue(UserIndicatorNumeric<T>::_controlValue);
	u8g_DrawBox(u8g, MIN(valueXCoord, _originXCoord), UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), abs(valueXCoord - _originXCoord), _barHeight);
	
	for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
	{
		int xCoord = xCoordinateFromValue(xVal);
		int tickLength = (((int)xVal) % 10 == 0 ? 3 : (((int)xVal) % 5 == 0 ? 2 : 1));
		u8g_DrawVLine(u8g, xCoord, UserIndicatorNumeric<T>::_locY - tickLength, tickLength);
		//u8g_DrawVLine(u8g, xCoord, UserIndicatorNumeric<T>::_locY + _barHeight, tickLength);
	}
}