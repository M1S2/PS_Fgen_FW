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
		UIElement* GetSelectedItem();
		void NextItem();
		void PreviousItem();
};

#endif /* PAGE_H_ */