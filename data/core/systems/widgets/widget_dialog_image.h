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


#ifndef __UNIGINE_WIDGET_DIALOG_IMAGE_H__
#define __UNIGINE_WIDGET_DIALOG_IMAGE_H__

#include <core/systems/widgets/widget_dialog.h>

namespace Unigine::Widgets
{
class DialogImage : Dialog
{
	WidgetDialogImage dialog_image;

	// constructor
	DialogImage()
	{
		dialog_image = new WidgetDialogImage(engine.getGui(), NULL);
		init_interface(0);
		dialog = dialog_image;
		widget = dialog;
	}

	DialogImage(int arg)
	{
		if (arg is Gui)
		{
			dialog_image = new WidgetDialogImage(arg, NULL);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			dialog_image = new WidgetDialogImage(engine.getGui(), arg);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		dialog = dialog_image;
		widget = dialog;
	}

	DialogImage(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			dialog_image = new WidgetDialogImage(arg1, arg2);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			dialog_image = new WidgetDialogImage(engine.getGui(), arg1);
			init_interface(arg2);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		dialog = dialog_image;
		widget = dialog;
	}

	DialogImage(Gui gui, string str, int flags)
	{
		dialog_image = new WidgetDialogImage(gui, str);
		init_interface(flags);
		dialog = dialog_image;
		widget = dialog;
	}

	// texture
	void setImage(Image image) { dialog_image.setImage(image); }
	Image getImage() { return dialog_image.getImage(); }
	void setTexture(string name) { dialog_image.setTexture(name); }
	string getTexture() { return dialog_image.getTexture(); }

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

int dialogImageGui(Gui gui, string str, string name)
{
	DialogImage dialog;

	// create dialog
	if (dialog == NULL)
	{
		dialog = new DialogImage(gui, str);
		dialog.setSizeable(1);
	}

	// dialog texture
	dialog.setText(str);
	dialog.setTexture(name);

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

int dialogImage(string str, string name)
{
	return dialogImageGui(engine.getGui(), str, name);
}

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_IMAGE_H__ */
