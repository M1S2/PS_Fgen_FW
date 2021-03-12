/*
 * UIElement.h
 *
 * Created: 12.03.2021 18:15:39
 *  Author: V17
 */ 


#ifndef UIELEMENT_H_
#define UIELEMENT_H_

#include "UIElementType.h"
#include "../libraries/u8glib/u8g.h"

class UIElement
{
	public:
		UIElementType Type;
		unsigned char LocX;
		unsigned char LocY;
		unsigned char Width;
		unsigned char Height;
		bool Visible;
		bool HasFocus;
		UIElement* Parent;

		UIElement(UIElementType type)
		{
			Visible = true;
			HasFocus = false;
			Type = type;
		}

		UIElement(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, UIElementType type)
		{
			LocX = locX;
			LocY = locY;
			Width = width;
			Height = height;
			Visible = true;
			HasFocus = false;
			Type = type;
		}

		virtual void Draw(u8g_t *u8g, bool isFirstPage) = 0;
};

#endif /* UIELEMENT_H_ */