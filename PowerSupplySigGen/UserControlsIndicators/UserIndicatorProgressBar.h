/*
 * UserIndicatorProgressBar.h
 *
 * Created: 15.01.2021 19:02:40
 *  Author: V17
 */ 

#ifndef USERINDICATORPROGRESSBAR_H_
#define USERINDICATORPROGRESSBAR_H_

#include "UserIndicatorNumeric.h"

template <class T>
class UserIndicatorProgressBar : public UserIndicatorNumeric<T>
{
private:
	T _minValue;
	T _maxValue;
	int _barWidth;
	int _barHeight;

public:
	UserIndicatorProgressBar(uint8_t locx, uint8_t locy, T* controlValue, const char* baseUnit, T minValue, T maxValue, int barWidth, int barHeight) : UserIndicatorNumeric<T>(locx, locy, controlValue, baseUnit)
    {
		_minValue = minValue;
		_maxValue = maxValue;
		_barWidth = barWidth;
		_barHeight = barHeight;
	}
        		
    void Draw(u8g_t *u8g)
	{
		UserIndicatorNumeric<T>::Draw(u8g);

		int valueWidth = ((*UserIndicatorNumeric<T>::_controlValue - _minValue) / (float)(_maxValue - _minValue)) * _barWidth;

		u8g_DrawFrame(u8g, UserIndicatorNumeric<T>::_locX + 60, UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), _barWidth, _barHeight);
		u8g_DrawBox(u8g, UserIndicatorNumeric<T>::_locX + 60 + (_minValue < 0 ? (_barWidth - valueWidth) : 0), UserIndicatorNumeric<T>::_locY + ((INDICATORS_FONT_HEIGHT - _barHeight) / 2), valueWidth, _barHeight);
	}
};


#endif /* USERINDICATORPROGRESSBAR_H_ */