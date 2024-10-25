/*
 * SplashScreen.cpp
 * Created: 02.03.2021 20:48:14
 * Author: Markus Scheich
 */ 

#include "ScreenManager.h"

#ifdef SPLASHSCREEN_ENABLED

#include <Fonts/FreeSansBold18pt7b.h>

Icon ico_logo(icon_powerCord_width, icon_powerCord_height, icon_powerCord_bits);
Label<20> lbl_projectName("PowerSupply Fgen", &FreeSansBold18pt7b);
Label<10> lbl_serialNo("SNo.: " SCPI_IDN_SERIAL_NUMBER);
Label<15> lbl_swVersion("SW: " SCPI_IDN_SOFTWARE_REVISION);
ContainerStack<4> stack_SplashScreen(STACK_LAYOUT_VERTICAL_CENTER);
ContainerGrid<1, 1, 1, false, true> grid_SplashScreenMain;

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