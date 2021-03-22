/*
 * BoolControl.h
 *
 * Created: 15.03.2021 19:12:14
 *  Author: V17
 */ 


#ifndef BOOLCONTROL_H_
#define BOOLCONTROL_H_

#include "BoolIndicator.h"

class BoolControl : public BoolIndicator
{
	public:
		BoolControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* valuePointer);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;

		void ToggleValue();
};

#endif /* BOOLCONTROL_H_ */