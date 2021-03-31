/*
 * UI_Manager.h
 *
 * Created: 12.03.2021 18:48:21
 *  Author: V17
 */ 


#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "UIElement.h"

class UI_Manager
{
	private:
		UIElement* _visualTreeRoot;
		UIElement* _focusElement;

		public:
		UI_Manager();
		void Init(u8g_t *u8g);

		void Draw(u8g_t *u8g, bool isFirstPage);
		void ChangeVisualTreeRoot(UIElement* visualTreeRoot);
		void SetFocusToLeaf();

		bool KeyInput(Keys_t key);
};

#endif /* UI_MANAGER_H_ */