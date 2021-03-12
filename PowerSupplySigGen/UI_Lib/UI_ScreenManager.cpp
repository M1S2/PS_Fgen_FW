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
	_visualTreeRoot->Draw(u8g, isFirstPage);
}

void UI_ScreenManager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	SetFocus(visualTreeRoot);
}

void UI_ScreenManager::SetFocus(UIElement* focusElement)
{
	if (_focusElement != NULL) { _focusElement->HasFocus = false; }
	_focusElement = focusElement;
	if (_focusElement != NULL) { _focusElement->HasFocus = true; }
}

void UI_ScreenManager::KeyInput(char key)
{
}

void UI_ScreenManager::EncoderInput(bool clockwise)
{
}

void UI_ScreenManager::EncoderPBInput()
{
}