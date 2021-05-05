/*
 * MessageDialog.cpp
 *
 * Created: 31.03.2021 20:55:40
 *  Author: V17
 */ 

#include "MessageDialog.h"
#include "../Indicators/Label.cpp"

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