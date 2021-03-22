/*
 * EnumControl.h
 *
 * Created: 15.03.2021 19:19:08
 *  Author: V17
 */ 


#ifndef ENUMCONTROL_H_
#define ENUMCONTROL_H_

#include "EnumIndicator.h"

template <class T>
class EnumControl : public EnumIndicator<T>
{	
	public:
		bool IsEditMode;
	
		EnumControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char** enumNames, uint8_t numEnumValues);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
			
		bool PreviousValue();
		bool NextValue();
		void ToggleEditMode();
};

#endif /* ENUMCONTROL_H_ */