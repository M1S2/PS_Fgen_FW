/*
 * BoolIndicator.h
 *
 * Created: 12.03.2021 18:25:18
 *  Author: V17
 */ 


#ifndef BOOLINDICATOR_H_
#define BOOLINDICATOR_H_

#include "UIElement.h"

class BoolIndicator : public UIElement
{
	private:
		bool* _controlValuePointer;
		bool _controlValue;

	public:
		BoolIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* controlValuePointer);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* BOOLINDICATOR_H_ */