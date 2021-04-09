/*
 * EnumControl.h
 *
 * Created: 15.03.2021 19:19:08
 *  Author: V17
 */ 


#ifndef ENUMCONTROL_H_
#define ENUMCONTROL_H_

#include "../Indicators/EnumIndicator.h"

template <class T>
class EnumControl : public EnumIndicator<T>
{	
	private:
		void* _controlContext;							// Context pointer that is returned with the _onValueChanged function pointer	
		void(*_onValueChanged)(void* controlContext);	// Function pointer for _onValueChanged event
			
	public:
		bool IsEditMode;
	
		EnumControl(unsigned char locX, unsigned char locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
			
		bool PreviousValue();
		bool NextValue();
		void ToggleEditMode();
};

#endif /* ENUMCONTROL_H_ */