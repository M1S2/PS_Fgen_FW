/*
 * UI_Lib_Test.h
 *
 * Created: 12.03.2021 18:57:41
 *  Author: V17
 */ 

#ifdef UI_LIB_TEST

#ifndef UI_LIB_TEST_H_
#define UI_LIB_TEST_H_

#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"

void UI_Test_Init(u8g_t *u8g);
void UI_Test_BuildTree();
void UI_Test_Draw(u8g_t *u8g, bool isFirstPage);
void UI_Test_KeyInput(Keys_t key);


/**** UI Icon Speed *****/
#define ui_icon_speed_width 16
#define ui_icon_speed_height 16
static unsigned char ui_icon_speed_bits[] U8G_PROGMEM = {
	0xC0, 0x03, 0x30, 0x0C, 0x4C, 0x32, 0x04, 0x20, 0x0A, 0x58, 0x02, 0x44,
	0x05, 0xA2, 0x01, 0x81, 0x81, 0x80, 0x05, 0xA0, 0x02, 0x40, 0x02, 0x40,
	0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

#endif /* UI_LIB_TEST_H_ */

#endif