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

bool Page::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			PreviousItem();
			return true;
		case KEYDOWN:
			NextItem();
			return true;
		default:
			return false;
	}
}

UIElement* Page::GetSelectedItem()
{
	return _items[_selectedItemIndex];
}

UIElement* Page::NextItem()
{
	_selectedItemIndex++;
	if (_selectedItemIndex >= _numItems) { _selectedItemIndex = 0; }
	ActiveChild = GetSelectedItem();
	return ActiveChild;
}

UIElement* Page::PreviousItem()
{
	_selectedItemIndex--;
	if (_selectedItemIndex < 0) { _selectedItemIndex = _numItems - 1; }
	ActiveChild = GetSelectedItem();
	return ActiveChild;
}