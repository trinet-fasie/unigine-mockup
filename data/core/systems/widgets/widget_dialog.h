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


#ifndef __UNIGINE_WIDGET_DIALOG_H__
#define __UNIGINE_WIDGET_DIALOG_H__

#include <core/systems/widgets/widget_button.h>

namespace Unigine::Widgets
{
class Dialog : Widget
{
	WidgetDialog dialog;

	Button ok_b;
	Button cancel_b;
	Button close_b;

#ifdef HAS_INTERFACE
	InterfaceWindow interface;
#endif

	// constructor
	Dialog()
	{
		dialog = new WidgetDialog(engine.getGui(), NULL, 0, 0);
		init_interface(0);
		widget = dialog;
	}

	Dialog(int arg)
	{
		if (arg is WidgetDialog)
		{
			owner = 0;
			dialog = arg;
			init_interface(0);
		} else if (arg is Gui)
		{
			dialog = new WidgetDialog(arg, NULL, 0, 0);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			dialog = new WidgetDialog(engine.getGui(), arg, 0, 0);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = dialog;
	}

	Dialog(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			dialog = new WidgetDialog(arg1, arg2, 0, 0);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			dialog = new WidgetDialog(engine.getGui(), arg1, arg2, 0);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = dialog;
	}

	Dialog(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, string, int)
		{
			dialog = new WidgetDialog(arg1, arg2, arg3, 0);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int, int)
		{
			dialog = new WidgetDialog(engine.getGui(), arg1, arg2, arg3);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = dialog;
	}

	Dialog(int arg1, int arg2, int arg3, int arg4)
	{
		if (arg1 is Gui) // (Gui, string, int, int)
		{
			dialog = new WidgetDialog(arg1, arg2, arg3, arg4);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int, int, int)
		{
			dialog = new WidgetDialog(engine.getGui(), arg1, arg2, arg3);
			init_interface(arg4);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = dialog;
	}

	Dialog(Gui gui, string str, int x, int y, int flags)
	{
		dialog = new WidgetDialog(gui, str, x, y);
		init_interface(flags);
		widget = dialog;
	}

	~Dialog()
	{
#ifdef HAS_INTERFACE
		delete interface;
#endif
	}

// interface
#ifdef HAS_INTERFACE
	InterfaceWindow getInterface()
	{
		return interface;
	}
#endif

	// movable flag
	void setMoveable(int moveable) { dialog.setMoveable(moveable); }
	int isMoveable() { return dialog.isMoveable(); }

	// sizeable flag
	void setSizeable(int sizeable) { dialog.setSizeable(sizeable); }
	int isSizeable() { return dialog.isSizeable(); }

	// space size
	void setSpace(int x, int y) { dialog.setSpace(x, y); }
	int getSpaceX() { return dialog.getSpaceX(); }
	int getSpaceY() { return dialog.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { dialog.setPadding(l, r, t, b); }
	int getPaddingLeft() { return dialog.getPaddingLeft(); }
	int getPaddingRight() { return dialog.getPaddingRight(); }
	int getPaddingTop() { return dialog.getPaddingTop(); }
	int getPaddingBottom() { return dialog.getPaddingBottom(); }

	// text
	void setText(string str) { dialog.setText(str); }
	string getText() { return dialog.getText(); }

	// result
	int isDone() { return dialog.isDone(); }
	int getResult() { return dialog.getResult(); }

	// ok button
	int isOkClicked() { return dialog.isOkClicked(); }
	void setOkText(string str) { dialog.setOkText(str); }
	string getOkText() { return dialog.getOkText(); }
	Button getOkButton()
	{
		if (ok_b == NULL)
			ok_b = new Button(dialog.getOkButton());
		return ok_b;
	}

	// cancel button
	int isCancelClicked() { return dialog.isCancelClicked(); }
	void setCancelText(string str) { dialog.setCancelText(str); }
	string getCancelText() { return dialog.getCancelText(); }
	Button getCancelButton()
	{
		if (cancel_b == NULL)
			cancel_b = new Button(dialog.getCancelButton());
		return cancel_b;
	}

	// close button
	int isCloseClicked() { return dialog.isCloseClicked(); }
	void setCloseText(string str) { dialog.setCloseText(str); }
	string getCloseText() { return dialog.getCloseText(); }
	Button getCloseButton()
	{
		if (close_b == NULL)
			close_b = new Button(dialog.getCloseButton());
		return close_b;
	}

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_H__ */
