/*
 * UIElement.h
 *
 * Created: 12.03.2021 18:15:39
 *  Author: V17
 */ 


#ifndef UIELEMENT_H_
#define UIELEMENT_H_

#include "UIElementType.h"
#include "Keys.h"
#include "../../libraries/u8glib/u8g.h"

#define DEFAULT_UI_ELEMENT_WIDTH	60
#define DEFAULT_UI_ELEMENT_HEIGHT	10

class UIElement
{
	public:
		UIElementType Type;
		unsigned char LocX;
		unsigned char LocY;
		unsigned char Width;
		unsigned char Height;
		bool Visible;
		UIElement* Parent;
		UIElement* ActiveChild;

		UIElement(UIElementType type)
		{
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}

		UIElement(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, UIElementType type)
		{
			LocX = locX;
			LocY = locY;
			Width = width;
			Height = height;
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}

		virtual void Draw(u8g_t *u8g, bool isFirstPage) = 0;
		virtual bool KeyInput(Keys_t key) { return false; }
};

#endif /* UIELEMENT_H_ */