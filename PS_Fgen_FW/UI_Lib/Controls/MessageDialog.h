/**
 * @file	MessageDialog.h
 * @date	31.03.2021 20:55:30
 * @author	Markus Scheich
 * @brief	Containing a class for a message dialog with optional Ok and Cancel buttons.
 */ 

#ifndef MESSAGEDIALOG_H_
#define MESSAGEDIALOG_H_

#include "../Core/UIElement.h"
#include "../Containers/ContainerPage.h"
#include "../Indicators/Label.h"
#include "../Indicators/Icon.h"
#include "../Controls/ButtonControl.h"

#define MAX_MESSAGEDIALOG_STRING_LENGTH		75			/**< Default string length for the message dialog text. */

/**
 * Available message dialog severity types.
 */
typedef enum MessageSeverity
{
	MSG_INFO,				/**< Message is an information. */
	MSG_WARNING,			/**< Message is a warning. */
	MSG_ERROR				/**< Message is an error. */
}MessageSeverity_t;

/**
 * Available message dialog buttons.
 */
typedef enum MessageButtons
{
	MSG_BTN_NONE,			/**< Dialog without any buttons. */
	MSG_BTN_OK,				/**< Dialog with one OK button. */
	MSG_BTN_OK_CANCEL		/**< Dialog with an OK and CANCEL button. */
}MessageButtons_t;

#define icon_info_width 16									/**< Width of the info icon. */
#define icon_info_height 16									/**< Height of the info icon. */
extern unsigned char icon_info_bits[] U8G_PROGMEM;			/**< Data array of the info icon. */
	
#define icon_warning_width 16								/**< Width of the warning icon. */
#define icon_warning_height 16								/**< Height of the warning icon. */
extern unsigned char icon_warning_bits[] U8G_PROGMEM;		/**< Data array of the warning icon. */

#define icon_error_width 16									/**< Width of the error icon. */
#define icon_error_height 16								/**< Height of the error icon. */
extern unsigned char icon_error_bits[] U8G_PROGMEM;			/**< Data array of the error icon. */

/**
 * Class for a message dialog with optional Ok and Cancel buttons.
 */
class MessageDialog : public UIElement
{
	private:
		ContainerPage _page;								/**< Container page that is internally used to group and handle all elements of the message dialog. */
		Label<MAX_MESSAGEDIALOG_STRING_LENGTH> _message;	/**< Label UIElement used to display the dialog message. */
		Icon _severityIcon;									/**< Icon UIElement used to display the message severity. */
		MessageSeverity_t _severity;						/**< Message severity. */
		ButtonControl<3> _buttonOk;							/**< Optional Ok button. */
		ButtonControl<7> _buttonCancel;						/**< Optional Cancel button. */

	public:
	
		/**
		 * Constructor of the MessageDialog.
		 * @param locX X Location of the upper left corner of the MessageDialog 
		 * @param locY Y Location of the upper left corner of the MessageDialog
		 * @param width Drawing width of the MessageDialog
		 * @param height Drawing height of the MessageDialog
		 * @param message Message string that is shown by this message dialog
		 * @param severity Message severity
		 * @param buttons Optional dialog buttons (None, Ok, Ok & Cancel)
		 * @param controlContext Context pointer that is returned with the onOkClick and onCancelClick function pointers
		 * @param onOkClick Function pointer for onOkClick event. This function is called when the Ok button is clicked.
		 * @param onCancelClick Function pointer for onCancelClick event. This function is called when the Cancel button is clicked.
		 */
		MessageDialog(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons = MSG_BTN_OK, void* controlContext = NULL, void(*onOkClick)(void* controlContext) = NULL, void(*onCancelClick)(void* controlContext) = NULL);
		
		/**
		 * Method used for drawing of the MessageDialog.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;

		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
};

#endif /* MESSAGEDIALOG_H_ */