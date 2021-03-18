/*
 * TabPage.cpp
 *
 * Created: 18.03.2021 18:45:53
 *  Author: V17
 */ 

#include "TabPage.h"
#include <string.h>

TabPage::TabPage(const char* header) : UIElement(UI_TABPAGE)
{
	strcpy(Header, header);
}

void TabPage::Draw(u8g_t *u8g, bool isFirstPage)
{
	if(_content != NULL) { _content->Draw(u8g, isFirstPage); }
}

void TabPage::SetContent(UIElement* content)
{
	if(content == NULL) { return; }
		
 	content->Parent = this;
	_content = content;
	ActiveChild = content;
}