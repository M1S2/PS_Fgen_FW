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
#include "NumericControl.cpp"
#include "ProgressBar.cpp"

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

Label labelBool(40, 5, 60, 10, "Boolean");
BoolIndicator boolInd1(40, 20, 60, 10, &boolVal1);
BoolControl boolCtrl1(40, 35, 60, 10, &boolVal1);
ContainerPage page_boolean;
Label labelEnum(40, 5, 60, 10, "Enumerations");
EnumIndicator<TestEnum> enumInd1(58, 20, 60, 10, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(58, 35, 60, 10, &enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(40, 35, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerPage page_enum;
ContainerList list1(40, 0, 240 - 40, 64);

Label labelNum(40, 5, 60, 10, "Numerics");
NumericIndicator<int> numInd2(40, 20, 60, 10, &numVal2, "A", 5000, 0);
NumericIndicator<float> numInd1(115, 20, 60, 10, &numVal1, "V", 2000, 3);
NumericControl<float> numCtrl1(115, 35, 60, 10, &numVal1, "V", -10, 2000, 3);
ProgressBar<float> progress1(115, 50, 100, 10, &numVal1, -10, 2000, PROGRESSBAR_ORIGIN_ZERO, 0);
ContainerPage page_numeric;

TabControl tabControl(0, 0, 240, 64, 32);

ContainerPage mainPage;
Label labelUILib(185, 5, 60, 10, "UI LIB", u8g_font_ncenB08r);
Icon speedIcon(185, 20, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);

void UI_Test_BuildTree()
{
	page_boolean.AddItem(&labelBool);
	page_boolean.AddItem(&boolInd1);
	page_boolean.AddItem(&boolCtrl1);
	page_boolean.InitItems();
	page_enum.AddItem(&labelEnum);
	page_enum.AddItem(&enumInd1);
	page_enum.AddItem(&enumCtrl1);
	page_enum.AddItem(&enumCtrl1Icon);
	page_enum.InitItems();	
	list1.AddItem(&page_boolean);
	list1.AddItem(&page_enum);
	
	page_numeric.AddItem(&labelNum);
	page_numeric.AddItem(&numInd1);
	page_numeric.AddItem(&numCtrl1);
	page_numeric.AddItem(&numInd2);
	page_numeric.AddItem(&progress1);
	page_numeric.InitItems();
	
	tabControl.AddTab("Tab1", &list1);
	tabControl.AddTab("Tab2", &page_numeric);
	tabControl.SelectTab(0);
	
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	mainPage.AddItem(&speedIcon);
	mainPage.InitItems();
	
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