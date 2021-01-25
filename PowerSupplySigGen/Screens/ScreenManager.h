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
#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"
#include "ScreenPS.h"
#include "ScreenDDS.h"
#include "ScreenDMM.h"
#include "ScreenATX.h"

#include "../Configuration.h"

class ScreenManagerClass
{
	private:
		ScreenPS _screenPs;
		ScreenDDS _screenDds;
		ScreenDMM _screenDmm;
		ScreenATX _screenAtx;
	
		u8g_t* _u8g;
		ScreenBase* _screens[NUM_SCREENS];
	    bool _isControlActive;
		bool _displayEnabled;
				
		void drawScreenTabs(int selectedTabIndex);
		void drawMessage();
		void drawSplashScreen();
		
	public:
		uint8_t TabIndex;
		bool DisplayInverted;
		char SystemMessage[MAX_MESSAGE_LEN];
		char UserMessage[MAX_MESSAGE_LEN];
	
		bool IsSplashScreenShown;
	
		ScreenManagerClass();
		void Draw(bool isFirstPage);
		
		void KeyInput(Keys_t key);
		void EncoderInput(EncoderDirection_t encDir);
		void EncoderPBInput();
		
		void SetDisplayEnabled(bool displayEnabled);
		bool GetDisplayEnabled();

		void SetDisplayInverted(bool displayInverted);

		void SetU8GLib_Object(u8g_t *u8g) { _u8g = u8g; }
};

extern ScreenManagerClass ScreenManager;

#endif /* SCREENMANAGER_H_ */