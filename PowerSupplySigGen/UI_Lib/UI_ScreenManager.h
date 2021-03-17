/*
 * UI_ScreenManager.h
 *
 * Created: 12.03.2021 18:48:21
 *  Author: V17
 */ 


#ifndef UI_SCREENMANAGER_H_
#define UI_SCREENMANAGER_H_

#include "UIElement.h"

class UI_ScreenManager
{
	private:
		UIElement* _visualTreeRoot;
		UIElement* _focusElement;

		public:
		UI_ScreenManager();

		void Draw(u8g_t *u8g, bool isFirstPage);
		void ChangeVisualTreeRoot(UIElement* visualTreeRoot);
		void SetFocusToLeaf();

		bool KeyInput(Keys_t key);
};

#endif /* UI_SCREENMANAGER_H_ */