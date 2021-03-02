/*
 * ScreenManager_SplashScreen.cpp
 *
 * Created: 02.03.2021 20:48:14
 *  Author: V17
 */ 

#include <stdio.h>
#include "ScreenManager.h"
#include "Icons.h"

#ifdef SPLASHSCREEN_ENABLED   

void ScreenManagerClass::drawSplashScreen()
{
	u8g_SetDefaultForegroundColor(&_u8g);
	u8g_DrawXBMP(&_u8g, 0, 0, icon_powerCord_width, icon_powerCord_height, icon_powerCord_bits);
	
	u8g_SetFont(&_u8g, u8g_font_profont22r);			// 14 pixel height font
	u8g_DrawStr(&_u8g, icon_powerCord_width + 5, 22, "PowerSupplySigGen");
	
	u8g_SetFont(&_u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	
	char buffer[50];
	sprintf(buffer, "by %s", SCPI_IDN_MANUFACTURER);
	u8g_DrawStr(&_u8g, 10, u8g_GetHeight(&_u8g) - 18, buffer);
	
	sprintf(buffer, "SNo.: %s", SCPI_IDN_SERIAL_NUMBER);
	u8g_DrawStr(&_u8g, 10, u8g_GetHeight(&_u8g) - 3, buffer);
	
	sprintf(buffer, "SW: %s", SCPI_IDN_SOFTWARE_REVISION);
	u8g_DrawStr(&_u8g, u8g_GetWidth(&_u8g) - 55, u8g_GetHeight(&_u8g) - 3, buffer);
}

#endif