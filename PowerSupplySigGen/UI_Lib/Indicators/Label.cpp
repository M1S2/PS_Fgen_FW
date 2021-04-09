/*
 * Label.cpp
 *
 * Created: 12.03.2021 18:19:51
 *  Author: V17
 */ 

#include "Label.h"
#include <string.h>

Label::Label(unsigned char locX, unsigned char locY, const char* text) : UIElement(locX, locY, UI_INDICATOR)
{
	strcpy(Text, text);
	_font = NULL;
}

Label::Label(unsigned char locX, unsigned char locY, const char* text, const u8g_fntpgm_uint8_t* font) : UIElement(locX, locY, UI_INDICATOR)
{
	strcpy(Text, text);
	_font = font;
}

void Label::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		const u8g_fntpgm_uint8_t* tmp_font;
		if(_font != NULL) 
		{
			tmp_font = u8g->font;
			u8g_SetFont(u8g, _font);
			u8g_SetFontPosTop(u8g);
		}
		
		u8g_DrawStrMultiline(u8g, LocX, LocY, Text);
		
		if(_font !=NULL) 
		{ 
			u8g_SetFont(u8g, tmp_font); 
			u8g_SetFontPosTop(u8g); 
		}
	}
}