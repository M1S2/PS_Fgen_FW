/*
 * UI_Lib_Test.cpp
 *
 * Created: 12.03.2021 18:56:45
 *  Author: V17
 */ 

#include "UI_Lib_Test.h"
#include "EnumControl.cpp"
#include "EnumIndicator.cpp"

enum TestEnum
{
	Test_A,
	Test_B,
	Test_C
};
const char* TestEnumNames[] = { "Test A", "Test B", "Test C" };

UI_ScreenManager ui_ScreenManager;
bool boolVal1;
TestEnum enumVal1;
Label label1(5, 5, 60, 10, "LABEL1");
BoolIndicator boolInd1(5, 15, 60, 10, &boolVal1);
BoolControl boolCtrl1(5, 30, 60, 10, &boolVal1);
EnumIndicator<TestEnum> enumInd1(80, 15, 60, 10, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(80, 30, 60, 10, &enumVal1, TestEnumNames, 3);
Page page1;

void UI_Test_BuildTree()
{
	page1.AddItem(&label1);
	page1.AddItem(&boolInd1);
	page1.AddItem(&boolCtrl1);
	page1.AddItem(&enumInd1);
	page1.AddItem(&enumCtrl1);	
	ui_ScreenManager.ChangeVisualTreeRoot(&page1);	
}

void UI_Test_Draw(u8g_t *u8g, bool isFirstPage)
{
	ui_ScreenManager.Draw(u8g, isFirstPage);
}

void UI_Test_KeyInput(Keys_t key)
{
	ui_ScreenManager.KeyInput(key);
}