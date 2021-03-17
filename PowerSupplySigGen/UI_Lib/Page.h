/*
 * Page.h
 *
 * Created: 12.03.2021 18:32:49
 *  Author: V17
 */ 


#ifndef PAGE_H_
#define PAGE_H_

#include "Container.h"

class Page : public Container
{
	private:
		int _selectedItemIndex;

	public:
		Page();
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
		
		UIElement* GetSelectedItem();
		UIElement* NextItem();
		UIElement* PreviousItem();
};

#endif /* PAGE_H_ */