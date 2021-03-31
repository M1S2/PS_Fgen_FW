/*
 * BoolControl.h
 *
 * Created: 15.03.2021 19:12:14
 *  Author: V17
 */ 


#ifndef BOOLCONTROL_H_
#define BOOLCONTROL_H_

#include "../Indicators/BoolIndicator.h"

class BoolControl : public BoolIndicator
{	
	private:
		void* _controlContext;							// Context pointer that is returned with the _onValueChanged function pointer
		void(*_onValueChanged)(void* controlContext);	// Function pointer for _onValueChanged event
	
	public:
		BoolControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* valuePointer, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;

		void ToggleValue();
};

#endif /* BOOLCONTROL_H_ */