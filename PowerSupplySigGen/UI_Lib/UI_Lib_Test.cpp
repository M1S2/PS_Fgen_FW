/*
 * UI_Lib_Test.cpp
 *
 * Created: 12.03.2021 18:56:45
 *  Author: V17
 */ 

#include "UI_Lib_Test.h"
#include "EnumControl.cpp"
#include "EnumIndicator.cpp"
#include "NumericIndicator.cpp"

enum TestEnum
{
	Test_A,
	Test_B,
	Test_C
};
const char* TestEnumNames[] = { "Test A", "Test B", "Test C" };

UI_Manager ui_Manager;
bool boolVal1;
TestEnum enumVal1;
float numVal1 = 23.456;
int numVal2 = 123;
Label label1(40, 5, 60, 10, "LABEL1");
BoolIndicator boolInd1(40, 20, 60, 10, &boolVal1);
BoolControl boolCtrl1(40, 35, 60, 10, &boolVal1);
EnumIndicator<TestEnum> enumInd1(115, 20, 60, 10, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(115, 35, 60, 10, &enumVal1, TestEnumNames, 3);
Page page1;

Label label2(40, 5, 60, 10, "LABEL2");
BoolControl boolCtrl2(40, 20, 60, 10, &boolVal1);
NumericIndicator<float> numInd1(115, 20, 60, 10, &numVal1, "V", 3, 3);
NumericIndicator<int> numInd2(40, 35, 60, 10, &numVal2, "A", 5, 0);
Page page2;

TabControl tabControl(0, 0, 240, 64, 32);

Page mainPage;
Label labelUILib(205, 5, 60, 10, "UI LIB");

void UI_Test_BuildTree()
{
	page1.AddItem(&label1);
	page1.AddItem(&boolInd1);
	page1.AddItem(&boolCtrl1);
	page1.AddItem(&enumInd1);
	page1.AddItem(&enumCtrl1);	
	
	page2.AddItem(&label2);
	page2.AddItem(&boolCtrl2);
	page2.AddItem(&numInd1);
	page2.AddItem(&numInd2);
	
	tabControl.AddTab("Tab1", &page1);
	tabControl.AddTab("Tab2", &page2);
	
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	
	ui_Manager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(u8g_t *u8g)
{
	ui_Manager.Init(u8g);
}

void UI_Test_Draw(u8g_t *u8g, bool isFirstPage)
{
	ui_Manager.Draw(u8g, isFirstPage);
}

void UI_Test_KeyInput(Keys_t key)
{
	ui_Manager.KeyInput(key);
}