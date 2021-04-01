/*
 * MessageDialog.cpp
 *
 * Created: 31.03.2021 20:55:40
 *  Author: V17
 */ 

#include "MessageDialog.h"

MessageDialog::MessageDialog(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, const char* message, MessageSeverity severity, bool showOkButton, void* controlContext, void(*onOkClick)(void* controlContext)) : UIElement(locX, locY, width, height, UI_CONTROL),
	_page(),
	_severityIcon(locX, locY, icon_info_width, icon_info_height, (severity == MSG_INFO ? icon_info_bits : (severity == MSG_WARNING ? icon_warning_bits : icon_error_bits))),
	_message(locX + icon_info_width + 5, locY, message),
	_buttonOk(locX + width / 2 - 10, locY + height - 12, 20, 10, "OK", controlContext, onOkClick)
{
	_severity = severity;
	_buttonOk.Visible = showOkButton;
	_page.AddItem(&_severityIcon);
	_page.AddItem(&_message);
	if (showOkButton) { _page.AddItem(&_buttonOk); }
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