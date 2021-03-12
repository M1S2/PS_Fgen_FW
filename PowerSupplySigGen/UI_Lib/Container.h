/*
 * Container.h
 *
 * Created: 12.03.2021 18:31:15
 *  Author: V17
 */ 


#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "UIElement.h"

#define MAX_CONTAINER_ITEMS		20

class Container : public UIElement
{
	protected:
		UIElement* _items[MAX_CONTAINER_ITEMS];
		int _numItems;

	public:
		Container();
		virtual void Draw(u8g_t *u8g, bool isFirstPage) = 0;
		void AddItem(UIElement* item);
};

#endif /* CONTAINER_H_ */