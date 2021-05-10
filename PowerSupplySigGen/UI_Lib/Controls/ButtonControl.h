/*
 * ButtonControl.h
 *
 * Created: 31.03.2021 20:42:27
 *  Author: V17
 */ 


#ifndef BUTTONCONTROL_H_
#define BUTTONCONTROL_H_

#include "../Core/UIElement.h"

#define DEFAULT_BUTTON_STRING_LENGTH		25

template <int StringLength>
class ButtonControl : public UIElement
{
	private:
	char _buttonText[StringLength];

	void* _controlContext;							// Context pointer that is returned with the _onClick function pointer
	void(*_onClick)(void* controlContext);			// Function pointer for _onClick event

	public:
	ButtonControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* buttonText, void* controlContext = NULL, void(*onClick)(void* controlContext) = NULL);
	virtual void Draw(u8g_t *u8g, bool isFirstPage) override;

	virtual bool KeyInput(Keys_t key) override;
};

typedef ButtonControl<DEFAULT_BUTTON_STRING_LENGTH> ButtonControlDefault;

#endif /* BUTTONCONTROL_H_ */