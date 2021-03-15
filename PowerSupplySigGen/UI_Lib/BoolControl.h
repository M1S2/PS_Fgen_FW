/*
 * BoolControl.h
 *
 * Created: 15.03.2021 19:12:14
 *  Author: V17
 */ 


#ifndef BOOLCONTROL_H_
#define BOOLCONTROL_H_

#include "UIElement.h"

class BoolControl : public UIElement
{
	private:
		bool* _controlValuePointer;
		bool _controlValueDraw;

	public:
		BoolControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* controlValuePointer);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;

		void ToggleValue();
};

#endif /* BOOLCONTROL_H_ */