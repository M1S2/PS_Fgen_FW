/*
 * EnumControl.h
 *
 * Created: 15.03.2021 19:19:08
 *  Author: V17
 */ 


#ifndef ENUMCONTROL_H_
#define ENUMCONTROL_H_

#include "UIElement.h"

template <class T>
class EnumControl : public UIElement
{
	private:
		const char** _enumNames;
		uint8_t _numEnumValues;
		T* _controlValuePointer;
		T _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
		
	public:
		bool IsEditMode;
	
		EnumControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* controlValuePointer, const char** enumNames, uint8_t numEnumValues);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
	
		bool PreviousValue();
		bool NextValue();
		void ToggleEditMode();
};

#endif /* ENUMCONTROL_H_ */