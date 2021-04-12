/*
 * ScreenSettings.cpp
 *
 * Created: 09.04.2021 19:17:59
 *  Author: V17
 */ 

#include "../Device.h"

// ***** Settings page *****
ContainerPage page_Settings;
Icon ico_settings(SCREEN_TAB_WIDTH + 5, 3, icon_settings_width, icon_settings_height, icon_settings_bits);
Label<10> lbl_Settings_caption(SCREEN_TAB_WIDTH + 25, 5, "Settings");

UIElement* uiBuildScreenSettings()
{
	page_Settings.AddItem(&ico_settings);
	page_Settings.AddItem(&lbl_Settings_caption);
	page_Settings.InitItems();
	
	return &page_Settings;
}
