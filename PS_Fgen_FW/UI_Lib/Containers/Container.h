/**
 * @file	Container.h
 * @date	12.03.2021 18:31:15
 * @author	Markus Scheich
 * @brief	Containing the base class for container elements.
 */ 

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../Core/UIElement.h"

#define MAX_CONTAINER_ITEMS		20		/**< Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory. */

/**
 * Base class for container elements.
 */
class Container : public UIElement
{
	protected:
		UIElement* _items[MAX_CONTAINER_ITEMS];		/**< Array holding pointers to all UIElement that are items of the container. */
		int _numItems;								/**< Number of items in the container (number of valid items in the _items array). */
		int _selectedItemIndex;						/**< Index of the selected container item. */

	public:
		/**
		 * Constructor of the Container.
		 */
		Container();
		
		/**
		 * Virtual method used for drawing of the container UIElement.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) { }  //= 0;
			
		/**
		 * Process the given key.
		 * This method can be overwritten in all derived classes. If it's not overwritten, no keys are supported by the container UIElement.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) { return false; }  // = 0;
		
		/**
		 * Get the selected item.
		 * This is the UIElement in the _items array at the _selectedItemIndex.
		 * @return Pointer to the selected UIElement in the container. 
		 */
		UIElement* GetSelectedItem();
		
		/**
		 * Add a new element to the container and activate it as active child.
		 * @param item Pointer to the UIElement that should be added to the container. 
		 */
		void AddItem(UIElement* item);
};

#endif /* CONTAINER_H_ */