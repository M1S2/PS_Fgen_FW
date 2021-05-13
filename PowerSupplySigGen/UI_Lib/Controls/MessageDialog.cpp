/*
 * MessageDialog.cpp
 *
 * Created: 31.03.2021 20:55:40
 *  Author: V17
 */ 

#include "MessageDialog.h"
#include "../Indicators/Label.cpp"
#include "../Controls/ButtonControl.cpp"

unsigned char icon_info_bits[] U8G_PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x84, 0x21, 0x82, 0x41, 0x02, 0x40,
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x82, 0x41, 0x82, 0x41,
	0x84, 0x21, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };

unsigned char icon_warning_bits[] U8G_PROGMEM = {
	0x80, 0x01, 0x40, 0x02, 0x40, 0x02, 0x20, 0x04, 0x20, 0x04, 0x90, 0x09,
	0x90, 0x09, 0x88, 0x11, 0x88, 0x11, 0x84, 0x21, 0x84, 0x21, 0x02, 0x40,
	0x82, 0x41, 0x81, 0x81, 0x01, 0x80, 0xfe, 0x7f };

unsigned char icon_error_bits[] U8G_PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x04, 0x20, 0x12, 0x48, 0x22, 0x44,
	0x41, 0x82, 0x81, 0x81, 0x81, 0x81, 0x41, 0x82, 0x22, 0x44, 0x12, 0x48,
	0x04, 0x20, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };

MessageDialog::MessageDialog(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons, void* controlContext, void(*onOkClick)(void* controlContext), void(*onCancelClick)(void* controlContext)) : UIElement(locX, locY, UI_CONTROL),
	_page(),
	_severityIcon(locX, locY, icon_info_width, icon_info_height, (severity == MSG_INFO ? icon_info_bits : (severity == MSG_WARNING ? icon_warning_bits : icon_error_bits))),
	_message(locX + icon_info_width + 5, locY, message),
	_buttonOk(locX + width / 2 - 20 - (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - 12, 40, 10, "OK", controlContext, onOkClick),
	_buttonCancel(locX + width / 2 - 20 + (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - 12, 40, 10, "Cancel", controlContext, onCancelClick)
{
	Width = width;
	Height = height;	
	_severity = severity;
	_page.AddItem(&_severityIcon);
	_page.AddItem(&_message);

	if (buttons == MSG_BTN_OK || buttons == MSG_BTN_OK_CANCEL) 
	{
		_buttonOk.Visible = true;
		_page.AddItem(&_buttonOk); 
	}
	if (buttons == MSG_BTN_OK_CANCEL)
	{
		_buttonCancel.Visible = true;
		_page.AddItem(&_buttonCancel);
	}

	_page.Parent = this;
	_page.InitItems();
	ActiveChild = &_page;
}

void MessageDialog::Draw(u8g_t *u8g, bool isFirstPage)
{
	_page.Draw(u8g, isFirstPage);
}

bool MessageDialog::KeyInput(Keys_t key)
{
	return _page.KeyInput(key);
}