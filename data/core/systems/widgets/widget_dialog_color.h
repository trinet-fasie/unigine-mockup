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


#ifndef __UNIGINE_WIDGET_DIALOG_COLOR_H__
#define __UNIGINE_WIDGET_DIALOG_COLOR_H__

#include <core/scripts/callback.h>
#include <core/systems/widgets/widget_dialog.h>

namespace Unigine::Widgets
{
class DialogColor : Dialog
{
	WidgetDialogColor dialog_color;

	// constructor
	DialogColor()
	{
		dialog_color = new WidgetDialogColor(engine.getGui(), NULL);
		init_interface(0);
		dialog = dialog_color;
		widget = dialog;
	}

	DialogColor(int arg)
	{
		if (arg is Gui)
		{
			dialog_color = new WidgetDialogColor(arg, NULL);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			dialog_color = new WidgetDialogColor(engine.getGui(), arg);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		dialog = dialog_color;
		widget = dialog;
	}

	DialogColor(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			dialog_color = new WidgetDialogColor(arg1, arg2);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			dialog_color = new WidgetDialogColor(engine.getGui(), arg1);
			init_interface(arg2);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		dialog = dialog_color;
		widget = dialog;
	}

	DialogColor(Gui gui, string str, int flags)
	{
		dialog_color = new WidgetDialogColor(gui, str);
		init_interface(flags);
		dialog = dialog_color;
		widget = dialog;
	}

	// path
	void setColor(vec4 color) { dialog_color.setColor(color); }
	vec4 getColor() { return dialog_color.getColor(); }

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

int dialogColorGui(Gui gui, string str, vec4 &color, CallbackBase callback)
{
	DialogColor dialog;

	// create dialog
	if (dialog == NULL)
		dialog = new DialogColor(gui, str);

	// dialog color
	dialog.setText(str);
	dialog.setColor(color);

	// show dialog
	dialog.arrange();
	addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);
	dialog.setPermanentFocus();

	// wait dialog
	while (dialog.isDone() == 0)
	{
		if (callback != NULL)
			callback.run(dialog.getColor());
		wait;
	}

	// dialog result
	if (dialog.isOkClicked())
		color = dialog.getColor();
	if (callback != NULL)
		callback.run(color);

	// hide dialog
	removeChild(dialog);

	return dialog.isOkClicked();
}

int dialogColor(string str, vec4 &color, CallbackBase callback = NULL)
{
	return dialogColorGui(engine.getGui(), str, color, callback);
}

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_COLOR_H__ */
