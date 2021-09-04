/*
 * Label.cpp
 * Created: 12.03.2021 18:19:51
 * Author: Markus Scheich
 */ 

#include "Label.h"
#include <string.h>

template <int StringLength>
Label<StringLength>::Label(unsigned char locX, unsigned char locY, const char* text) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
}

template <int StringLength>
Label<StringLength>::Label(unsigned char locX, unsigned char locY, const char* text, const u8g_fntpgm_uint8_t* font) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = font;
}

template <int StringLength>
void Label<StringLength>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		const u8g_fntpgm_uint8_t* tmp_font = u8g->font;
		if(_font != NULL) 
		{
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

template <int StringLength>
void Label<StringLength>::SetText(const char* text)
{
	strncpy(Text, text, StringLength);		// Copy a maximum number of StringLength characters to the Text buffer. If text is shorter, the array is zero padded.
	Text[StringLength - 1] = '\0';			// The Text buffer must contain at least one termination character ('\0') at the end to protect from overflow.
}