/*
 * TabPage.h
 *
 * Created: 18.03.2021 18:45:43
 *  Author: V17
 */ 


#ifndef TABPAGE_H_
#define TABPAGE_H_

#include "UIElement.h"

#define MAX_TAB_PAGE_HEADER_LENGTH		50

class TabPage : public UIElement
{
	private:
		UIElement* _content;

	public:
		char Header[MAX_TAB_PAGE_HEADER_LENGTH];

		TabPage(const char* header);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;

		void SetContent(UIElement* content);
};

#endif /* TABPAGE_H_ */