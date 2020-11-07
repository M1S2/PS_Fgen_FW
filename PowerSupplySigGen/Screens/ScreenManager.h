/*
 * ScreenManager.h
 *
 * Created: 05.11.2020 18:45:19
 *  Author: V17
 */ 


#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include "../GLCD/u8g.h"
#include "ScreenBase.h"
#include "../Global/DevSettings.h"
#include "../Global/DevStatus.h"

#define NUM_SCREENS				5

class ScreenManagerClass
{
	private:
		u8g_t* _u8g;
		ScreenBase* _screens[NUM_SCREENS];
		void drawScreenTabs(int selectedTabIndex);
	public:
		ScreenManagerClass();
		void Draw(DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw);

		void SetU8GLib_Object(u8g_t *u8g) { _u8g = u8g; }
};

extern ScreenManagerClass ScreenManager;


#endif /* SCREENMANAGER_H_ */