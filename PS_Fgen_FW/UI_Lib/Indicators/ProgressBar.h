/*
 * ProgressBar.h
 *
 * Created: 26.03.2021 18:17:46
 *  Author: V17
 */ 


#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "../Core/UIElement.h"

typedef enum ProgressBarOrigin
{
	PROGRESSBAR_ORIGIN_LEFT,
	PROGRESSBAR_ORIGIN_ZERO,
	PROGRESSBAR_ORIGIN_RIGHT
}ProgressBarOrigin_t;

template <class T>
class ProgressBar : public UIElement
{
	private:
		T* _valuePointer;
		T _valueDraw;
		T _minValue;
		T _maxValue;
		ProgressBarOrigin_t _origin;
		T _tickIncrement;				// Set this to 0 (or negative) to disable ticks

		unsigned char _originXCoord;
		unsigned char xCoordinateFromValue(T value);

	public:
		ProgressBar(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* PROGRESSBAR_H_ */