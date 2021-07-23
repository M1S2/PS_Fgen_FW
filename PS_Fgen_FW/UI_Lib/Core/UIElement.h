/**
 * @file	UIElement.h
 * @date	12.03.2021 18:15:39
 * @author	Markus Scheich
 * @brief	Containing the abstract base class for all user interface elements (controls, indicators, containers).
 */ 

#ifndef UIELEMENT_H_
#define UIELEMENT_H_

#include "UIElementType.h"
#include "Keys.h"
#include "../../libraries/u8glib/u8g.h"

#define DEFAULT_UI_ELEMENT_WIDTH	60			/**< Default width for an UIElement */
#define DEFAULT_UI_ELEMENT_HEIGHT	10			/**< Default height for an UIElement */

/**
 * Abstract base class for all user interface elements (controls, indicators, containers).
 */
class UIElement
{
	public:
		UIElementType Type;			/**< Element type (control, indicator, container) */
		unsigned char LocX;			/**< X Location of the upper left corner of the UIElement */
		unsigned char LocY;			/**< Y Location of the upper left corner of the UIElement */
		unsigned char Width;		/**< Drawing width of the UIElement */
		unsigned char Height;		/**< Drawing height of the UIElement */
		bool Visible;				/**< The UIElement is only drawn if the visibility is set to true */
		UIElement* Parent;			/**< Parent UIElement that this UIElement belongs to */
		UIElement* ActiveChild;		/**< Child element that is currently active (receiving all key inputs). This property is only set for elements that contain children (mostly containers). Simple controls or indicators should set this to NULL. */

		/**
		 * Constructor of the UIElement.
		 * @param type Element type (control, indicator, container)
		 */
		UIElement(UIElementType type)
		{
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}
	
		/**
		 * Constructor of the UIElement.
		 * @param locX X Location of the upper left corner of the UIElement
		 * @param locY Y Location of the upper left corner of the UIElement
		 * @param type Element type (control, indicator, container)
		 */
		UIElement(unsigned char locX, unsigned char locY, UIElementType type)
		{
			LocX = locX;
			LocY = locY;
			Width = DEFAULT_UI_ELEMENT_WIDTH;
			Height = DEFAULT_UI_ELEMENT_HEIGHT;
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}

		/**
		 * Virtual method used for drawing of the UIElement.
		 * This method must be overwritten in all derived classes.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) = 0;
		
		/**
		 * Process the given key.
		 * This method can be overwritten in all derived classes. If it's not overwritten, no keys are supported by the UIElement.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) { return false; }
};

#endif /* UIELEMENT_H_ */