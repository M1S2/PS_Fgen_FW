/*
 * UI_Lib_Test.h
 *
 * Created: 12.03.2021 18:57:41
 *  Author: V17
 */ 


#ifndef UI_LIB_TEST_H_
#define UI_LIB_TEST_H_

#include "UI_ScreenManager.h"
#include "UI_Elements.h"
#include "../KeyPad/KeyPad.h"

void UI_Test_BuildTree();
void UI_Test_Draw(u8g_t *u8g, bool isFirstPage);
void UI_Test_KeyInput(Keys_t key);

#endif /* UI_LIB_TEST_H_ */