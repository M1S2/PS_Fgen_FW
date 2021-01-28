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
	
	int xCoordinateFromValue(T value);
	
public:
	UserIndicatorProgressBar(uint8_t locx, uint8_t locy, T* controlValuePointer, const char* baseUnit, T minValue, T maxValue, int barWidth, int barHeight, ProgressBarOrigin_t origin, float tickIncrement);
    void Draw(u8g_t *u8g, bool isFirstPage);
};


#endif /* USERINDICATORPROGRESSBAR_H_ */