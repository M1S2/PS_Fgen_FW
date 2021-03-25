/*
 * Label.h
 *
 * Created: 12.03.2021 18:19:10
 *  Author: V17
 */ 


#ifndef LABEL_H_
#define LABEL_H_

#include "UIElement.h"

#define MAX_LABEL_STRING_LENGTH		50

class Label : public UIElement
{
	private:
		const u8g_fntpgm_uint8_t* _font;
	
	public:
		char Text[MAX_LABEL_STRING_LENGTH];

		Label(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* text);
		Label(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* text, const u8g_fntpgm_uint8_t* font);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* LABEL_H_ */