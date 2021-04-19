/*
 * TabControl.h
 *
 * Created: 18.03.2021 18:50:55
 *  Author: V17
 */ 


#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "../Core/UIElement.h"

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
		
		void* _controlContext;									// Context pointer that is returned with the _onSelectedTabChanged function pointer
		void(*_onSelectedTabChanged)(void* controlContext);		// Function pointer for _onSelectedTabChanged event

	public:
		TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth, void* controlContext = NULL, void(*onSelectedTabChanged)(void* controlContext) = NULL);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
		
		void AddTab(const char* header, UIElement* tabContent);
		void NextTab();
		void PreviousTab();
		void SelectTab(int index);
		int GetSelectedTabIndex();
};

#endif /* TABCONTROL_H_ */