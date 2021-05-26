/*
 * Label.h
 *
 * Created: 12.03.2021 18:19:10
 *  Author: V17
 */ 


#ifndef LABEL_H_
#define LABEL_H_

#include "../Core/UIElement.h"

#define DEFAULT_LABEL_STRING_LENGTH		50

template <int StringLength>
class Label : public UIElement
{
	private:
		const u8g_fntpgm_uint8_t* _font;
	
	public:
		char Text[StringLength];

		Label(unsigned char locX, unsigned char locY, const char* text);
		Label(unsigned char locX, unsigned char locY, const char* text, const u8g_fntpgm_uint8_t* font);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		void SetText(const char* text);
};

typedef Label<DEFAULT_LABEL_STRING_LENGTH> LabelDefault;

#endif /* LABEL_H_ */