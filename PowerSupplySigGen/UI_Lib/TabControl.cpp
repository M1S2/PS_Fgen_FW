/*
 * TabControl.cpp
 *
 * Created: 18.03.2021 18:51:05
 *  Author: V17
 */ 

#include "TabControl.h"
#include <string.h>

TabControl::TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth) : UIElement(locX, locY, width, height, UI_CONTROL)
{
	_numTabs = 0;
	_selectedTabIndex = 0;
	_tabWidth = tabWidth;
}

void TabControl::Draw(u8g_t *u8g, bool isFirstPage) 
{
	if (Visible)
	{
		u8g_DrawFrame(u8g, LocX + _tabWidth - 1, LocY, Width - _tabWidth + 1, Height);
		int yTab = LocY;
		
		int tabFontHeight = u8g_GetFontAscent(u8g) - u8g_GetFontDescent(u8g);
		int tabHeight = tabFontHeight + 4;
		
		for(int i = 0; i < _numTabs; i++)
		{
			if(i == _selectedTabIndex)
			{
				u8g_DrawFrame(u8g, LocX, yTab, _tabWidth, tabHeight);
				u8g_SetDefaultBackgroundColor(u8g);
				u8g_DrawVLine(u8g, LocX + _tabWidth - 1, yTab + 1, tabHeight - 2);
				u8g_SetDefaultForegroundColor(u8g);				
			}
			
			if(_headers[i] != NULL) { u8g_DrawStr(u8g, LocX + 2, yTab + ((tabHeight - tabFontHeight) / 2), _headers[i]); }
			yTab+=(tabHeight + TABCONTROL_TABPAGE_MARGIN);
		}
		
		_tabContents[_selectedTabIndex]->Draw(u8g, isFirstPage);
	}
}

bool TabControl::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			NextTab();
			return true;
		case KEYRIGHT:
			PreviousTab();
			return true;
		default:
			return false;
	}
}

void TabControl::AddTab(const char* header, UIElement* tabContent)
{
	if (_numTabs >= MAX_TABCONTROL_TABS) { return; }

	tabContent->Parent = this;
	_tabContents[_numTabs] = tabContent;
	strcpy(_headers[_numTabs], header);
	_numTabs++;

	if (ActiveChild == NULL) { ActiveChild = tabContent; }
}

void TabControl::NextTab()
{
	_selectedTabIndex++;
	SelectTab(_selectedTabIndex);
}

void TabControl::PreviousTab()
{
	_selectedTabIndex--;
	SelectTab(_selectedTabIndex);
}

void TabControl::SelectTab(int index)
{	
	_selectedTabIndex = index;
	if (_selectedTabIndex >= _numTabs) { _selectedTabIndex = 0; }
	else if (_selectedTabIndex < 0) { _selectedTabIndex = _numTabs - 1; }
	
	ActiveChild = _tabContents[_selectedTabIndex];
}

int TabControl::GetSelectedTabIndex()
{
	return _selectedTabIndex;
}