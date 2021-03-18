/*
 * TabControl.cpp
 *
 * Created: 18.03.2021 18:51:05
 *  Author: V17
 */ 

#include "TabControl.h"

TabControl::TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth) : UIElement(locX, locY, width, height, UI_TABCONTROL)
{
	_numPages = 0;
	_selectedPageIndex = 0;
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
		
		for(int i = 0; i < _numPages; i++)
		{
			if(i == _selectedPageIndex)
			{
				u8g_DrawFrame(u8g, LocX, yTab, _tabWidth, tabHeight);
				u8g_SetDefaultBackgroundColor(u8g);
				u8g_DrawVLine(u8g, LocX + _tabWidth - 1, yTab + 1, tabHeight - 2);
				u8g_SetDefaultForegroundColor(u8g);				
			}
			
			if(_pages[i] != NULL) { u8g_DrawStr(u8g, LocX + 2, yTab + ((tabHeight - tabFontHeight) / 2), _pages[i]->Header); }
			yTab+=(tabHeight + TABCONTROL_TABPAGE_MARGIN);
		}
		
		_pages[_selectedPageIndex]->Draw(u8g, isFirstPage);
	}
}

bool TabControl::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			NextPage();
			return true;
		case KEYRIGHT:
			PreviousPage();
			return true;
		default:
			return false;
	}
}

void TabControl::AddPage(TabPage* page)
{
	if (_numPages >= MAX_TABCONTROL_TABPAGES) { return; }

	page->Parent = this;
	_pages[_numPages] = page;
	_numPages++;

	if (ActiveChild == NULL) { ActiveChild = page; }
}

void TabControl::NextPage()
{
	_selectedPageIndex++;
	if (_selectedPageIndex >= _numPages) { _selectedPageIndex = 0; }
	ActiveChild = _pages[_selectedPageIndex];
}

void TabControl::PreviousPage()
{
	_selectedPageIndex--;
	if (_selectedPageIndex < 0) { _selectedPageIndex = _numPages - 1; }
	ActiveChild = _pages[_selectedPageIndex];
}