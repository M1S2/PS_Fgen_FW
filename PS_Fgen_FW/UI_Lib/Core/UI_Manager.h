/**
 * @file	UI_Manager.h
 * @date	12.03.2021 18:48:21
 * @author	Markus Scheich
 * @brief	Containing a class that is used to handle the drawing and key handling of all UI_Elements.
 */ 

#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "UIElement.h"

/**
 * Class that is used to handle the drawing and key handling of all UI_Elements.
 */
class UI_Manager
{
	private:
		UIElement* _visualTreeRoot;					/**< Root element of the visual tree. This element and all children are drawn to the screen. */
		UIElement* _focusElement;					/**< Element that has the focus (it is highlited and receives all key inputs). */

		/**
		 * Traverse down the visual tree until an element without a child is reached and focus this element.
		 */
		void setFocusToLeaf();

	public:
	
		/** Constructor of the UI_Manager */
		UI_Manager();
		
		/**
		 * Initialize the UI_Manager.
		 * This method sets the font and foreground color of the graphic library.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 */
		void Init(u8g_t *u8g);

		/**
		 * Draw the complete visual tree (_visualTreeRoot and all of its children).
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.  
		 */
		void Draw(u8g_t *u8g, bool isFirstPage);

		/**
		 * Change the root element of the visual tree. 
		 * This changes the _visualTreeRoot variable and recalculates the _focusElement.
		 * @param visualTreeRoot UIElement that becomes the new root element of the visual tree. 
		 */
		void ChangeVisualTreeRoot(UIElement* visualTreeRoot);
		
		/**
		 * Forward the given key to the _focusElement.
		 * If the element type of the _focusElement doesn't support the key type, the key is forwarded to the parent of the _focusElement.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if no control in the visual tree has processed the key.
		 */
		bool KeyInput(Keys_t key);
};

#endif /* UI_MANAGER_H_ */