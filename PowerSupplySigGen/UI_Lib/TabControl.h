/*
 * TabControl.h
 *
 * Created: 18.03.2021 18:50:55
 *  Author: V17
 */ 


#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "UIElement.h"

#define MAX_TABCONTROL_TABS			5
#define MAX_HEADER_LENGTH			10
#define TABCONTROL_TABPAGE_MARGIN	0

class TabControl : public UIElement
{
	private:
		UIElement* _tabContents[MAX_TABCONTROL_TABS];
		char _headers[MAX_TABCONTROL_TABS][MAX_HEADER_LENGTH];
		int _numTabs;
		int _selectedTabIndex;
		unsigned char _tabWidth;

	public:
		TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
		
		void AddTab(const char* header, UIElement* tabContent);
		void NextTab();
		void PreviousTab();
};

#endif /* TABCONTROL_H_ */