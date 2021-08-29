/**
 * @file	UI_Icons.h
 * @date	29.08.2021 15:30:21
 * @author	Markus Scheich
 * @brief	Containing icons for the UI_Lib.
 */ 

#ifndef UI_ICONS_H_
#define UI_ICONS_H_

#include "../Core/UIElement.h"

#define icon_info_width 16									/**< Width of the info icon. */
#define icon_info_height 16									/**< Height of the info icon. */
extern unsigned char icon_info_bits[] U8G_PROGMEM;			/**< Data array of the info icon. */

#define icon_warning_width 16								/**< Width of the warning icon. */
#define icon_warning_height 16								/**< Height of the warning icon. */
extern unsigned char icon_warning_bits[] U8G_PROGMEM;		/**< Data array of the warning icon. */

#define icon_error_width 16									/**< Width of the error icon. */
#define icon_error_height 16								/**< Height of the error icon. */
extern unsigned char icon_error_bits[] U8G_PROGMEM;			/**< Data array of the error icon. */

#endif /* UI_ICONS_H_ */