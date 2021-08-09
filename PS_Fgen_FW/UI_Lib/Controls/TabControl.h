/**
 * @file	TabControl.h
 * @date	18.03.2021 18:50:55
 * @author	Markus Scheich
 * @brief	Containing a class for a tab control that is showing a vertical tab layout and can hold child elements.
 */ 

#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "../Core/UIElement.h"

#define MAX_TABCONTROL_TABS			5		/**< Maximum number of tabs that are supported. */
#define MAX_HEADER_LENGTH			8		/**< Maximum string length of the tab headers. */
#define TABCONTROL_TABPAGE_MARGIN	0		/**< Margin around the tab page tabs. */

/**
 * Class for a tab control that is showing a tab layout and can hold child elements.
 */
class TabControl : public UIElement
{
	private:
		UIElement* _tabContents[MAX_TABCONTROL_TABS];				/**< Array with pointers to the tab page contents which can of any UIElement. */
		char _headers[MAX_TABCONTROL_TABS][MAX_HEADER_LENGTH];		/**< Array with all tab page tab header strings. */
		int _numTabs;												/**< Number of Tabs. */
		int _selectedTabIndex;										/**< Index of the currently selected tab. */
		unsigned char _tabWidth;									/**< Width of the Tabs in pixel. */
		
		void* _controlContext;										/**< Context pointer that is returned with the _onSelectedTabChanged function pointer */
		void(*_onSelectedTabChanged)(void* controlContext);			/**< Function pointer for _onSelectedTabChanged event. This function is called when the selected tab is changed. */
		
	public:
	
		/**
		 * Constructor of the TabControl.
		 * @param locX X Location of the upper left corner of the TabControl 
		 * @param locY Y Location of the upper left corner of the TabControl
		 * @param width Drawing width of the TabControl
		 * @param height Drawing height of the TabControl
		 * @param tabWidth Width of the Tabs in pixel.
		 * @param controlContext Context pointer that is returned with the _onSelectedTabChanged function pointer
		 * @param onSelectedTabChanged Function pointer for _onSelectedTabChanged event. This function is called when the selected tab is changed.
		 */
		TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth, void* controlContext = NULL, void(*onSelectedTabChanged)(void* controlContext) = NULL);
	
		/**
		 * Method used for drawing of the TabControl.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYLEFT, KEYRIGHT
s		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
		
		/**
		 * Add a new tab to the tab control.
		 * @param header Tab page tab header string.
		 * @param tabContent Pointer to the tab page content which can of any UIElement.
		 */
		void AddTab(const char* header, UIElement* tabContent);
		
		/**
		 * Select the next tab page.
		 * If the current page is the last one, the first page is selected again (to implement a circular behaviour).
		 */
		void NextTab();
		
		/**
		 * Select the previous tab page.
		 * If the current page is the first one, the last page is selected again (to implement a circular behaviour).
		 */
		void PreviousTab();
		
		/**
		 * Select the tab at the requested index (after range checking).
		 * @param index Index of the tab page that should be selected.
		 */
		void SelectTab(int index);
		
		/**
		 * Return the value of the selected tab index.
		 */
		int GetSelectedTabIndex();
};

#endif /* TABCONTROL_H_ */