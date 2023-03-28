/*
 * ContainerPage.cpp
 * Created: 12.03.2021 18:33:36
 * Author: Markus Scheich
 */ 

#include "ContainerPage.h"

ContainerPage::ContainerPage()
{
	Type = UI_CONTAINER;
}

void ContainerPage::Draw(u8g_t *u8g, bool isFirstPage)
{
	for (int i = 0; i < _numItems; i++)
	{
		_items[i]->Draw(u8g, isFirstPage);
	}
}

bool ContainerPage::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextControlItem();
		case KEYDOWN:
			return PreviousControlItem();
		default:
			return false;
	}
}

void ContainerPage::InitItems()
{
	_selectedItemIndex = 0;
	if (GetSelectedItem()->Type != UI_CONTROL || GetSelectedItem()->Visible == false)
	{
		NextControlItem();
	}
}

bool ContainerPage::NextItem()
{
	if (_selectedItemIndex >= _numItems - 1)
	{
		return false;
	}
	
	while (_selectedItemIndex < _numItems - 1)
	{
		_selectedItemIndex++;
		ActiveChild = GetSelectedItem();
		if(ActiveChild != NULL && ActiveChild->Visible == true) { return true; }
	}
	_selectedItemIndex--;	// Decrement index to stay in range (was incremented in the loop before)
	ActiveChild = GetSelectedItem();
	return false;
}

bool ContainerPage::PreviousItem()
{
	if (_selectedItemIndex <= 0)
	{
		return false;
	}
	
	while (_selectedItemIndex > 0)
	{
		_selectedItemIndex--;
		ActiveChild = GetSelectedItem();
		if(ActiveChild != NULL && ActiveChild->Visible == true) { return true; }
	}
	_selectedItemIndex++;	// Increment index to stay in range (was decremented in the loop before)
	ActiveChild = GetSelectedItem();
	return false;
}

bool ContainerPage::NextControlItem()
{
	int nextControlIndex = -1;
	for (int i = (_selectedItemIndex + 1); i < _numItems; i++)
	{
		if (_items[i]->Type == UI_CONTROL && _items[i]->Visible == true)
		{
			nextControlIndex = i;
			break;
		}
	}
	
	if (nextControlIndex != -1)
	{
		_selectedItemIndex = nextControlIndex;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}

bool ContainerPage::PreviousControlItem()
{
	int previousControlIndex = -1;
	for (int i = (_selectedItemIndex - 1); i >= 0; i--)
	{
		if (_items[i]->Type == UI_CONTROL && _items[i]->Visible == true)
		{
			previousControlIndex = i;
			break;
		}
	}
	
	if (previousControlIndex != -1)
	{
		_selectedItemIndex = previousControlIndex;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}