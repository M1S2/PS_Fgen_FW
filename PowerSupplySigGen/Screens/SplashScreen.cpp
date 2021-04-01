/*
 * SplashScreen.cpp
 *
 * Created: 02.03.2021 20:48:14
 *  Author: V17
 */ 

#include "ScreenManager.h"

#ifdef SPLASHSCREEN_ENABLED   

Icon ico_logo(0, 0, icon_powerCord_width, icon_powerCord_height, icon_powerCord_bits);
Label lbl_projectName(icon_powerCord_width + 5, 8, "PowerSupplySigGen", u8g_font_profont22r);
Label lbl_manufacturer(10, 64 - 26, strcat("by ", SCPI_IDN_MANUFACTURER));
Label lbl_serialNo(10, 64 - 11, strcat("SNo.: ", SCPI_IDN_SERIAL_NUMBER));
Label lbl_swVersion(240 - 55, 64 - 11, strcat("SW: ", SCPI_IDN_SOFTWARE_REVISION));
ContainerPage page_SplashScreen;

UIElement* uiBuildSplashScreen()
{
	page_SplashScreen.AddItem(&ico_logo);
	page_SplashScreen.AddItem(&lbl_projectName);
	page_SplashScreen.AddItem(&lbl_manufacturer);
	page_SplashScreen.AddItem(&lbl_serialNo);
	page_SplashScreen.AddItem(&lbl_swVersion);
	page_SplashScreen.InitItems();
	
	return &page_SplashScreen;
}

#endif