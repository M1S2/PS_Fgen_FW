/*
 * ScreenBase.h
 *
 * Created: 05.11.2020 18:47:03
 *  Author: V17
 */ 


#ifndef SCREENBASE_H_
#define SCREENBASE_H_

#include "../GLCD/u8g.h"
#include "../Global/DevStatus.h"
#include "../UserControls/UserControlBase.h"
#include <stdlib.h>
#include <string.h>

#include "../Configuration.h"

class ScreenBase
{
	protected:
		UserControlBase* _userControls[MAX_SCREEN_USER_CONTROLS];
		char _numUserControls;
		char _selectedUserControl;

	public:
		const char* TabName;
	
		ScreenBase(const char* tabName) : TabName(tabName)
		{
			_selectedUserControl = 0;
		}
		
		void KeyInput(Keys_t key);
		void EncoderInput(EncoderDirection_t encDir, bool isControlActive);
		void EncoderPBInput();
		
		void ActivateSelectedControl(bool active);
		bool IsSelectedControlActive();
		
		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENBASE_H_ */