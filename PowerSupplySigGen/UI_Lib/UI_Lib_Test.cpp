/*
 * UI_Lib_Test.cpp
 *
 * Created: 12.03.2021 18:56:45
 *  Author: V17
 */ 

#include "UI_Lib_Test.h"

UI_ScreenManager ui_ScreenManager;
bool boolVal1;
Label label1(0, 0, 0, 0, "LABEL1");
BoolIndicator boolInd1(0, 20, 0, 0, &boolVal1);

void UI_Test_BuildTree()
{
	ui_ScreenManager.ChangeVisualTreeRoot(&label1);	
}

void UI_Test_Draw(u8g_t *u8g, bool isFirstPage)
{
	ui_ScreenManager.Draw(u8g, isFirstPage);
}