/*
 * ContainerList.cpp
 *
 * Created: 29.03.2021 18:46:37
 *  Author: V17
 */ 

#include "ContainerList.h"

ContainerList::ContainerList(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
}

void ContainerList::Draw(u8g_t *u8g, bool isFirstPage)
{
	GetSelectedItem()->Draw(u8g, isFirstPage);
	
	// Draw scroll bar
	unsigned char scrollBarLeft = LocX + Width - SCROLLBAR_WIDTH - SCROLLBAR_MARGIN;
	unsigned char scrollBarBoxHeight = Height - 2 * SCROLLBAR_MARGIN;
	unsigned char scrollBarHeight = scrollBarBoxHeight / _numItems;
	u8g_DrawFrame(u8g, scrollBarLeft, LocY + SCROLLBAR_MARGIN, SCROLLBAR_WIDTH, scrollBarBoxHeight);
	u8g_DrawBox(u8g, scrollBarLeft, LocY + SCROLLBAR_MARGIN + (_selectedItemIndex * scrollBarHeight), SCROLLBAR_WIDTH, scrollBarHeight);
}

bool ContainerList::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextItem();
		case KEYDOWN:
			return PreviousItem();
		default:
			return false;
	}
}

bool ContainerList::NextItem()
{
	if (_selectedItemIndex < _numItems - 1)
	{
		_selectedItemIndex++;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}

bool ContainerList::PreviousItem()
{
	if (_selectedItemIndex > 0)
	{
		_selectedItemIndex--;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}