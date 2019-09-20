/* Copyright (C) 2005-2019, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.9 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#ifndef __UNIGINE_WIDGET_DIALOG_MESSAGE_H__
#define __UNIGINE_WIDGET_DIALOG_MESSAGE_H__

#include <core/systems/widgets/widget_dialog.h>

namespace Unigine::Widgets
{
class DialogMessage : Dialog
{
	WidgetDialogMessage dialog_message;

	// constructor
	DialogMessage()
	{
		dialog_message = new WidgetDialogMessage(engine.getGui(), NULL);
		init_interface(0);
		dialog = dialog_message;
		widget = dialog;
	}

	DialogMessage(int arg)
	{
		if (arg is Gui)
		{
			dialog_message = new WidgetDialogMessage(arg, NULL);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			dialog_message = new WidgetDialogMessage(engine.getGui(), arg);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		dialog = dialog_message;
		widget = dialog;
	}

	DialogMessage(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			dialog_message = new WidgetDialogMessage(arg1, arg2);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			dialog_message = new WidgetDialogMessage(engine.getGui(), arg1);
			init_interface(arg2);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		dialog = dialog_message;
		widget = dialog;
	}

	DialogMessage(Gui gui, string str, int flags)
	{
		dialog_message = new WidgetDialogMessage(gui, str);
		init_interface(flags);
		dialog = dialog_message;
		widget = dialog;
	}

	// message hidden
	void setMessageHidden(int hidden) { dialog_message.setMessageHidden(hidden); }
	int isMessageHidden() { return dialog_message.isMessageHidden(); }

	// message font
	void setMessageFont(string name) { dialog_message.setMessageFont(name); }
	void setMessageFontSize(int size) { dialog_message.setMessageFontSize(size); }
	void setMessageFontColor(vec4 color) { dialog_message.setMessageFontColor(color); }
	void setMessageFontRich(int rich) { dialog_message.setMessageFontRich(rich); }

	// message text
	void setMessageText(string str) { dialog_message.setMessageText(str); }
	string getMessageText() { return dialog_message.getMessageText(); }

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

int dialogMessageGui(Gui gui, string str, string text)
{
	DialogMessage dialog;

	// create dialog
	if (dialog == NULL)
		dialog = new DialogMessage(gui, str);

	// dialog message
	dialog.setText(str);
	dialog.setMessageText(text);

	// show dialog
	dialog.arrange();
	addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);
	dialog.setPermanentFocus();

	// wait dialog
	while (dialog.isDone() == 0)
		wait;

	// hide dialog
	removeChild(dialog);

	return dialog.isOkClicked();
}

int dialogMessageYesNoGui(Gui gui, string str, string text)
{
	return dialogMessageGui(gui, str, text);
}

int dialogMessage(string str, string text)
{
	return dialogMessageGui(engine.getGui(), str, text);
}

int dialogMessageYesNo(string str, string text)
{
	return dialogMessageYesNoGui(engine.getGui(), str, text);
}

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_MESSAGE_H__ */
