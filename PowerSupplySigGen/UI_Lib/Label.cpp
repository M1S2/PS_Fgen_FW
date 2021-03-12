/*
 * Label.cpp
 *
 * Created: 12.03.2021 18:19:51
 *  Author: V17
 */ 

#include "Label.h"
#include <string.h>

Label::Label(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* text) : UIElement(locX, locY, width, height, UI_LABEL)
{
	strcpy(Text, text);
}

void Label::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		u8g_DrawStr(u8g, LocX, LocY, Text);
	}
}