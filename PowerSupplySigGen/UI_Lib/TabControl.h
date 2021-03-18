/*
 * TabControl.h
 *
 * Created: 18.03.2021 18:50:55
 *  Author: V17
 */ 


#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "UIElement.h"
#include "TabPage.h"

#define MAX_TABCONTROL_TABPAGES		5
#define TABCONTROL_TABPAGE_MARGIN	0

class TabControl : public UIElement
{
	private:
		TabPage* _pages[MAX_TABCONTROL_TABPAGES];
		int _numPages;
		int _selectedPageIndex;
		unsigned char _tabWidth;

	public:
		TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;

		void AddPage(TabPage* page);
		void NextPage();
		void PreviousPage();
};

#endif /* TABCONTROL_H_ */