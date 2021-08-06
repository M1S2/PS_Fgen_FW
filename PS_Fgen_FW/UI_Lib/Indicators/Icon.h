/**
 * @file	Icon.h
 * @date	26.03.2021 18:54:58
 * @author	Markus Scheich
 * @brief	Containing a class for an icon indicator that is drawing an icon to the screen.
 */ 

#ifndef ICON_H_
#define ICON_H_

#include "../Core/UIElement.h"

/**
 * Class for an icon indicator that is drawing an icon to the screen.
 */
class Icon : public UIElement
{
	private:
		const u8g_pgm_uint8_t* _iconBits;			/**< Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] U8G_PROGMEM = { ... }` */
	
	public:
	
		/**
		 * Constructor of the Icon.
		 * @param locX X Location of the upper left corner of the Icon 
		 * @param locY Y Location of the upper left corner of the Icon
		 * @param iconWidth Drawing width of the Icon
 		 * @param iconHeight Drawing height of the Icon
		 * @param iconBits Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] U8G_PROGMEM = { ... }`
		 */
		Icon(unsigned char locX, unsigned char locY, unsigned char iconWidth, unsigned char iconHeight, const u8g_pgm_uint8_t* iconBits);
		
		/**
		 * Method used for drawing of the Icon.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* ICON_H_ */