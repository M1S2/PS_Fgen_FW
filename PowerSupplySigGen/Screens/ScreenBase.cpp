/*
 * ScreenBase.cpp
 *
 * Created: 09.11.2020 18:57:19
 *  Author: V17
 */ 

#include "ScreenBase.h"

void ScreenBase::KeyInput(Keys_t key)
{
	_userControls[_selectedUserControl]->KeyInput(key);
}

void ScreenBase::EncoderInput(EncoderDirection_t encDir, bool isControlActive)
{
	if(isControlActive)
	{
		_userControls[_selectedUserControl]->EncoderInput(encDir);
	}
	else
	{
		if(encDir)
		{
			_selectedUserControl++;
			if(_selectedUserControl >= _numUserControls) { _selectedUserControl = 0; }
		}
		else
		{
			_selectedUserControl--;
			if(_selectedUserControl < 0) { _selectedUserControl = _numUserControls; }
		}
		
		for(int i=0; i<_numUserControls; i++)
		{
			_userControls[i]->IsSelected = false;
		}
		_userControls[_selectedUserControl]->IsSelected = true;
	}
}

void ScreenBase::EncoderPBInput()
{
	_userControls[_selectedUserControl]->EncoderPBInput();
}

void ScreenBase::ActivateSelectedControl(bool active)
{
	_userControls[_selectedUserControl]->IsActive = active;
}

bool ScreenBase::IsSelectedControlActive()
{
	return _userControls[_selectedUserControl]->IsActive;
}

void ScreenBase::Draw(u8g_t* u8g, DevStatus_t devStatusDraw)
{
	for(int i = 0; i < _numUserControls; i++)
	{
		if(_userControls[i] != NULL) { _userControls[i]->Draw(u8g); }
	}
}
