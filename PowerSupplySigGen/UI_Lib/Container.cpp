/*
 * Container.cpp
 *
 * Created: 12.03.2021 18:32:11
 *  Author: V17
 */ 

#include "Container.h"

Container::Container() : UIElement(UI_CONTAINER)
{
	_numItems = 0;
}

void Container::AddItem(UIElement* item)
{
	if (_numItems >= MAX_CONTAINER_ITEMS) { return; }

	item->Parent = this;
	_items[_numItems] = item;
	_numItems++;
}