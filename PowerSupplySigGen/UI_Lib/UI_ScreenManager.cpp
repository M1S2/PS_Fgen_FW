/*
 * UI_Element.cpp
 *
 * Created: 12.03.2021 18:49:21
 *  Author: V17
 */ 

#include "UI_ScreenManager.h"
#include "UI_Elements.h"

UI_ScreenManager::UI_ScreenManager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
}

void UI_ScreenManager::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (_visualTreeRoot == NULL) { return; }
	
	if(_focusElement != NULL) { u8g_DrawFrame(u8g, _focusElement->LocX - 1, _focusElement->LocY - 1, _focusElement->Width + 2, _focusElement->Height + 2); }
	_visualTreeRoot->Draw(u8g, isFirstPage);
}

void UI_ScreenManager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	SetFocusToLeaf();
}

// Traverse down the visual tree until an element without a child is reached and focus this element.
void UI_ScreenManager::SetFocusToLeaf()
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

bool UI_ScreenManager::KeyInput(Keys_t key)
{
	SetFocusToLeaf();
		
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
	return keyProcessed;
}