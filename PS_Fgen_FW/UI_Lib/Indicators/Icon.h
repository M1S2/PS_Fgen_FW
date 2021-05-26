/*
 * Icon.h
 *
 * Created: 26.03.2021 18:54:58
 *  Author: V17
 */ 


#ifndef ICON_H_
#define ICON_H_

#include "../Core/UIElement.h"

class Icon : public UIElement
{
	private:
		const u8g_pgm_uint8_t* _iconBits;
	
	public:
		Icon(unsigned char locX, unsigned char locY, unsigned char iconWidth, unsigned char iconHeight, const u8g_pgm_uint8_t* iconBits);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* ICON_H_ */