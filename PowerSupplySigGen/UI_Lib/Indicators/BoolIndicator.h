/*
 * BoolIndicator.h
 *
 * Created: 12.03.2021 18:25:18
 *  Author: V17
 */ 


#ifndef BOOLINDICATOR_H_
#define BOOLINDICATOR_H_

#include "../Core/UIElement.h"

class BoolIndicator : public UIElement
{
	protected:
		bool* _valuePointer;
		bool _valueDraw;			// This variable is updated from the _valuePointer on each draw of the first page.

	public:
		BoolIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* valuePointer);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* BOOLINDICATOR_H_ */