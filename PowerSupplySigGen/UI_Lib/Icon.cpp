/*
 * Icon.cpp
 *
 * Created: 26.03.2021 18:55:07
 *  Author: V17
 */ 

#include "Icon.h"

Icon::Icon(unsigned char locX, unsigned char locY, unsigned char iconWidth, unsigned char iconHeight, const u8g_pgm_uint8_t* iconBits) : UIElement(locX, locY, iconWidth, iconHeight, UI_ICON)
{
	_iconBits = iconBits;
}

void Icon::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		if(_iconBits != NULL)
		{
			u8g_DrawXBMP(u8g, LocX, LocY, Width, Height, _iconBits);
		}
	}
}