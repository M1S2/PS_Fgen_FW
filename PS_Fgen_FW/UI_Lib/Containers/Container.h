/*
 * Container.h
 *
 * Created: 12.03.2021 18:31:15
 *  Author: V17
 */ 


#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../Core/UIElement.h"

#define MAX_CONTAINER_ITEMS		20

class Container : public UIElement
{
	protected:
		UIElement* _items[MAX_CONTAINER_ITEMS];
		int _numItems;
		int _selectedItemIndex;

	public:
		Container();
		virtual void Draw(u8g_t *u8g, bool isFirstPage) { }  //= 0;
		virtual bool KeyInput(Keys_t key) { return false; }  // = 0;
		
		UIElement* GetSelectedItem();
		void AddItem(UIElement* item);
};

#endif /* CONTAINER_H_ */