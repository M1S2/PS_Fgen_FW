/*
 * UI_Lib_Test.cpp
 *
 * Created: 12.03.2021 18:56:45
 *  Author: V17
 */ 

#include "UI_Lib_Test.h"
#include "Controls/EnumControl.cpp"
#include "Indicators/EnumIndicator.cpp"
#include "Indicators/NumericIndicator.cpp"
#include "Controls/NumericControl.cpp"
#include "Indicators/ProgressBar.cpp"

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

void OnBoolVal1Changed(void* context);
void OnNumVal1Changed(void* context);
void OnButtonReset(void* context);
void OnMsgOk(void* context);
void OnShowError(void* context);

Label labelBool(40, 5, "Boolean");
BoolIndicator boolInd1(40, 20, &boolVal1);
BoolControl boolCtrl1(40, 35, &boolVal1, &boolVal1, &OnBoolVal1Changed);
ContainerPage page_boolean;
Label labelEnum(40, 5, "Enumerations");
EnumIndicator<TestEnum> enumInd1(58, 20, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(58, 35, &enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(40, 35, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerPage page_enum;
ContainerList list1(40, 0, 240 - 40, 64);

Label labelNum(40, 5, "Numerics");
NumericIndicator<int> numInd2(40, 20, &numVal2, "A", 5000, 0);
NumericIndicator<float> numInd1(115, 20, &numVal1, "V", 2000, 3);
NumericControl<float> numCtrl1(115, 35, &numVal1, "V", -10, 2000, 3, &numVal1, &OnNumVal1Changed);
ProgressBar<float> progress1(115, 50, 100, 10, &numVal1, -10, 2000, PROGRESSBAR_ORIGIN_ZERO, 0);
ContainerPage page_numeric;

ButtonControl buttonReset(40, 20, 60, 10, "Reset", NULL, &OnButtonReset);
ButtonControl buttonShowTestError(40, 35, 60, 10, "Show Error", NULL, &OnShowError);
MessageDialog msgReset(5, 5, 240 - 5, 64 - 5, "Reset sucessful.", MSG_INFO, true, NULL, &OnMsgOk);
MessageDialog msgTestWarning(5, 5, 240 - 5, 64 - 5, "Warning message.\nWith Newline.", MSG_WARNING, true, NULL, &OnMsgOk);
MessageDialog msgTestError(5, 5, 240 - 5, 64 - 5, "Error message.", MSG_ERROR, true, NULL, &OnMsgOk);
ContainerPage page_dialogs;

TabControl tabControl(0, 0, 240, 64, 32);

ContainerPage mainPage;
Label labelUILib(185, 5, "UI LIB", u8g_font_ncenB08r);
Icon speedIcon(185, 20, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);

void OnBoolVal1Changed(void* context)
{
	bool boolVal = *((bool*)context);
	speedIcon.Visible = boolVal;
	ui_Manager.ChangeVisualTreeRoot(&msgTestWarning);
}

void OnNumVal1Changed(void* context)
{
	numVal2++;	
}

void OnButtonReset(void* context)
{
	boolVal1 = false;
	enumVal1 = Test_A;
	numVal1 = 0;
	numVal2 = 0;
	ui_Manager.ChangeVisualTreeRoot(&msgReset);
}

void OnShowError(void* context)
{
	ui_Manager.ChangeVisualTreeRoot(&msgTestError);
}

void OnMsgOk(void* context)
{
	ui_Manager.ChangeVisualTreeRoot(&mainPage);	
}

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
	
	page_dialogs.AddItem(&buttonReset);
	page_dialogs.AddItem(&buttonShowTestError);
	page_dialogs.InitItems();
	
	tabControl.AddTab("Tab1", &list1);
	tabControl.AddTab("Tab2", &page_numeric);
	tabControl.AddTab("Tab3", &page_dialogs);
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