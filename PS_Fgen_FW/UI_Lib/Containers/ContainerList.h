/**
 * @file	ContainerList.h
 * @date	29.03.2021 18:46:29
 * @author	Markus Scheich
 * @brief	Containing a class for a container that is showing one item at a time and supports scrolling.
 */ 

#ifndef CONTAINERLIST_H_
#define CONTAINERLIST_H_

#include "Container.h"

#define SCROLLBAR_WIDTH			4		/**< Width in pixel of the scroll bar on the right side of the container */
#define SCROLLBAR_MARGIN		2		/**< Margin in pixel between the scroll bar frame and the scroll bar slider */

/**
 * Class for a container that is showing one item at a time and supports scrolling.
 */
class ContainerList : public Container
{
	public:
		/**
		 * Constructor of the ContainerList.
		 * @param locX X Location of the upper left corner of the ContainerList 
		 * @param locY Y Location of the upper left corner of the ContainerList
		 * @param width Drawing width of the ContainerList
		 * @param height Drawing height of the ContainerList
		 */
		ContainerList(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height);
		
		/**
		 * Method used for drawing of the ContainerList.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
			
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. The following keys are supported: KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Scroll to the next item in the container.
		 * @return true if there was a next item (if the selected item wasn't the last one).
		 */
		bool NextItem();
		
		/**
		 * Scroll to the previous item in the container.
		 * @return true if there was a previous item (if the selected item wasn't the first one).
		 */
		bool PreviousItem();
};

#endif /* CONTAINERLIST_H_ */