/*
 * SplashScreen.cpp
 * Created: 02.03.2021 20:48:14
 * Author: Markus Scheich
 */ 

#include "ScreenManager.h"

#ifdef SPLASHSCREEN_ENABLED

#include <Fonts/FreeSansBold18pt7b.h>

Icon ico_logo(icon_powerCord_width, icon_powerCord_height, icon_powerCord_bits);
Label lbl_projectName("PowerSupply Fgen", LABEL_COLOR_NOTSET, &FreeSansBold18pt7b, 0, 0, 20);
Label lbl_serialNo("SNo.: " SCPI_IDN_SERIAL_NUMBER, LABEL_COLOR_NOTSET, NULL, 0, 0, 10);
Label lbl_swVersion("SW: " SCPI_IDN_SOFTWARE_REVISION, LABEL_COLOR_NOTSET, NULL, 0, 0, 15);
ContainerStack stack_SplashScreen(STACK_LAYOUT_VERTICAL_CENTER, 4);
ContainerGrid grid_SplashScreenMain(1, 1, 1, false, true);

UIElement* uiBuildSplashScreen()
{
	stack_SplashScreen.AddItem(&ico_logo);
	stack_SplashScreen.AddItem(&lbl_projectName);
	stack_SplashScreen.AddItem(&lbl_serialNo);
	stack_SplashScreen.AddItem(&lbl_swVersion);

	// This grid is used to center the splash screen vertically
	grid_SplashScreenMain.SetColumnWidth(0, DISPLAY_WIDTH);
	grid_SplashScreenMain.SetRowHeight(0, DISPLAY_HEIGHT);
	grid_SplashScreenMain.AddItem(&stack_SplashScreen, 0, 0, GRID_CELL_ALIGNMENT_MIDDLE);

	return &grid_SplashScreenMain;
}

#endif