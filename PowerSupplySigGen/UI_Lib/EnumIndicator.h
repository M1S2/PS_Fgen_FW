/*
 * EnumIndicator.h
 *
 * Created: 15.03.2021 19:42:12
 *  Author: V17
 */ 


#ifndef ENUMINDICATOR_H_
#define ENUMINDICATOR_H_

#include "UIElement.h"

template <class T>
class EnumIndicator : public UIElement
{
	private:
		const char** _enumNames;
		uint8_t _numEnumValues;
		T* _controlValuePointer;
		T _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
	
	public:
	
		EnumIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* controlValuePointer, const char** enumNames, uint8_t numEnumValues);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* ENUMINDICATOR_H_ */