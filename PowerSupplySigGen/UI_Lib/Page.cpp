/*
 * Page.cpp
 *
 * Created: 12.03.2021 18:33:36
 *  Author: V17
 */ 

#include "Page.h"

Page::Page()
{
	Type = UI_PAGE;
}

void Page::Draw(u8g_t *u8g, bool isFirstPage)
{
	for (int i = 0; i < _numItems; i++)
	{
		_items[i]->Draw(u8g, isFirstPage);
	}
}

UIElement* Page::GetSelectedItem()
{
	return _items[_selectedItemIndex];
}

void Page::NextItem()
{
	_selectedItemIndex++;
	if (_selectedItemIndex >= _numItems) { _selectedItemIndex = 0; }
}

void Page::PreviousItem()
{
	_selectedItemIndex--;
	if (_selectedItemIndex < 0) { _selectedItemIndex = _numItems - 1; }
}