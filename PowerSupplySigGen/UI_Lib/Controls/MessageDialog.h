/*
 * MessageDialog.h
 *
 * Created: 31.03.2021 20:55:30
 *  Author: V17
 */ 


#ifndef MESSAGEDIALOG_H_
#define MESSAGEDIALOG_H_

#include "../Core/UIElement.h"
#include "../Containers/ContainerPage.h"
#include "../Indicators/Label.h"
#include "../Indicators/Icon.h"
#include "../Controls/ButtonControl.h"

#define MAX_MESSAGEDIALOG_STRING_LENGTH		256

typedef enum MessageSeverity
{
	MSG_INFO,
	MSG_WARNING,
	MSG_ERROR
}MessageSeverity_t;

typedef enum MessageButtons
{
	MSG_BTN_NONE,
	MSG_BTN_OK,
	MSG_BTN_OK_CANCEL
}MessageButtons_t;

#define icon_info_width 16
#define icon_info_height 16
static unsigned char icon_info_bits[] U8G_PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x84, 0x21, 0x82, 0x41, 0x02, 0x40,
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x82, 0x41, 0x82, 0x41,
	0x84, 0x21, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };
	
#define icon_warning_width 16
#define icon_warning_height 16
static unsigned char icon_warning_bits[] U8G_PROGMEM = {
	0x80, 0x01, 0x40, 0x02, 0x40, 0x02, 0x20, 0x04, 0x20, 0x04, 0x90, 0x09,
	0x90, 0x09, 0x88, 0x11, 0x88, 0x11, 0x84, 0x21, 0x84, 0x21, 0x02, 0x40,
	0x82, 0x41, 0x81, 0x81, 0x01, 0x80, 0xfe, 0x7f };

#define icon_error_width 16
#define icon_error_height 16
static unsigned char icon_error_bits[] U8G_PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x04, 0x20, 0x12, 0x48, 0x22, 0x44,
	0x41, 0x82, 0x81, 0x81, 0x81, 0x81, 0x41, 0x82, 0x22, 0x44, 0x12, 0x48,
	0x04, 0x20, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };
	
class MessageDialog : public UIElement
{
	private:
		ContainerPage _page;
		Label<MAX_MESSAGEDIALOG_STRING_LENGTH> _message;
		Icon _severityIcon;
		MessageSeverity_t _severity;
		ButtonControl _buttonOk;
		ButtonControl _buttonCancel;

	public:
		MessageDialog(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons = MSG_BTN_OK, void* controlContext = NULL, void(*onOkClick)(void* controlContext) = NULL, void(*onCancelClick)(void* controlContext) = NULL);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;

		virtual bool KeyInput(Keys_t key) override;
};

#endif /* MESSAGEDIALOG_H_ */