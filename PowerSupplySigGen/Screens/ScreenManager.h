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
#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"
#include "ScreenPS.h"
#include "ScreenDMM.h"
#include "ScreenATX.h"

#define NUM_SCREENS			5
#define MESSAGE_MARGIN		5
#define MAX_MESSAGE_LEN		256

class ScreenManagerClass
{
	private:
		ScreenPS _screenPs;
		ScreenDMM _screenDmm;
		ScreenATX _screenAtx;
	
		u8g_t* _u8g;
		ScreenBase* _screens[NUM_SCREENS];
	    bool _isControlActive;
		bool _displayEnabled;
				
		void drawScreenTabs(int selectedTabIndex);
		void drawMessage();
		
	public:
		uint8_t TabIndex;
		char* SystemMessage;
		char UserMessage[MAX_MESSAGE_LEN];
	
		ScreenManagerClass();
		void Draw(DevStatus_t devStatusDraw);
		void KeyInput(Keys_t key);
		void EncoderInput(EncoderDirection_t encDir);
		void EncoderPBInput();
		
		void SetDisplayEnabled(bool displayEnabled);
		bool GetDisplayEnabled();

		void SetU8GLib_Object(u8g_t *u8g) { _u8g = u8g; }
};

extern ScreenManagerClass ScreenManager;


#endif /* SCREENMANAGER_H_ */