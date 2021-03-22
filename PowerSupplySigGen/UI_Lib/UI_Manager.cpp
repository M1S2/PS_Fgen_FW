/*
 * UI_Manager.cpp
 *
 * Created: 12.03.2021 18:49:21
 *  Author: V17
 */ 

#include "UI_Manager.h"
#include "UI_Elements.h"

UI_Manager::UI_Manager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
}

void UI_Manager::Init(u8g_t *u8g)
{
	u8g_SetFont(u8g, u8g_font_helvR08r);		// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFontPosTop(u8g);
}

void UI_Manager::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (_visualTreeRoot == NULL) { return; }
	
	if(_focusElement != NULL) { u8g_DrawFrame(u8g, _focusElement->LocX - 1, _focusElement->LocY - 1, _focusElement->Width + 2, _focusElement->Height + 2); }
	_visualTreeRoot->Draw(u8g, isFirstPage);
}

void UI_Manager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	SetFocusToLeaf();
}

// Traverse down the visual tree until an element without a child is reached and focus this element.
void UI_Manager::SetFocusToLeaf()
{
	if (_visualTreeRoot == NULL) { return; }
	
	UIElement* visualTreeElement = _visualTreeRoot;
	UIElement* leaf = NULL;

	while (visualTreeElement != NULL)
	{
		leaf = visualTreeElement;
		visualTreeElement = visualTreeElement->ActiveChild;		// Traverse down the tree
	}
	_focusElement = leaf;
}

bool UI_Manager::KeyInput(Keys_t key)
{
	bool keyProcessed = false;
	UIElement* visualTreeElement = _focusElement;
	while (!keyProcessed && visualTreeElement != NULL)
	{
		keyProcessed = visualTreeElement->KeyInput(key);
		if (!keyProcessed)
		{
			visualTreeElement = visualTreeElement->Parent;		// Traverse up the tree
		}
	}
	SetFocusToLeaf();
	return keyProcessed;
}