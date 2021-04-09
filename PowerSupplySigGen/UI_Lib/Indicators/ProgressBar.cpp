/*
 * ProgressBar.cpp
 *
 * Created: 26.03.2021 18:17:56
 *  Author: V17
 */ 

#include "ProgressBar.h"
#include <math.h>

template <class T>
unsigned char ProgressBar<T>::xCoordinateFromValue(T value)
{
	return LocX + (unsigned char)(((value - _minValue) / (float)(_maxValue - _minValue)) * (Width - 1));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
ProgressBar<T>::ProgressBar(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement): UIElement(locX, locY, UI_INDICATOR)
{
	Width = width;
	Height = height;
	_valuePointer = valuePointer;
	_minValue = minValue;
	_maxValue = maxValue;
	_origin = origin;
	_tickIncrement = tickIncrement;

	_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
}

template <class T>
void ProgressBar<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage) 
		{ 
			_valueDraw = *_valuePointer; 
			if (_valueDraw > _maxValue) { _valueDraw = _maxValue; }			// Coerce value to be between _minValue and _maxValue
			else if (_valueDraw < _minValue) { _valueDraw = _minValue; }
		}

		unsigned char valueXCoord = xCoordinateFromValue(_valueDraw);
		
		// Draw outer border of progress bar
		u8g_DrawFrame(u8g, xCoordinateFromValue(_minValue), LocY, Width, Height);

		// Change font for min and max value strings
		const u8g_fntpgm_uint8_t* tmp_font;
		tmp_font = u8g->font;
		u8g_SetFont(u8g, u8g_font_5x7r);
		u8g_SetFontPosTop(u8g);
				
		char buffer[6];
		itoa(_maxValue, buffer, 10);
		u8g_DrawStr(u8g, xCoordinateFromValue(_maxValue) + 3, LocY, buffer);
		itoa(_minValue, buffer, 10);
		u8g_DrawStr(u8g, xCoordinateFromValue(_minValue) - 15, LocY, buffer);

		// Change back font to previous font
		u8g_SetFont(u8g, tmp_font);
		u8g_SetFontPosTop(u8g);

		u8g_DrawBox(u8g, (unsigned char)fmin(valueXCoord, _originXCoord), LocY, (unsigned char)fabs(valueXCoord - _originXCoord), Height);
		
		if(_tickIncrement != 0)			// Use _tickIncrement=0 to disable ticks
		{
			for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
			{
				int xCoord = xCoordinateFromValue(xVal);
				int tickLength = (((int)xVal) % 10 == 0 ? 3 : (((int)xVal) % 5 == 0 ? 2 : 1));
				u8g_DrawVLine(u8g, xCoord, LocY - tickLength, tickLength);
				//u8g_DrawVLine(u8g, xCoord, LocY + Height, tickLength);
			}
		}
	}
}