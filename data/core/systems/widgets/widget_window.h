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


#ifndef __UNIGINE_WIDGET_WINDOW_H__
#define __UNIGINE_WIDGET_WINDOW_H__

namespace Unigine::Widgets
{
class Window : Widget
{
	WidgetWindow window;

#ifdef HAS_INTERFACE
	InterfaceWindow interface;
#endif

	// constructor
	Window()
	{
		window = new WidgetWindow(engine.getGui(), NULL, 0, 0);
		init_interface(0);
		widget = window;
	}

	Window(int arg)
	{
		if (arg is WidgetWindow)
		{
			owner = 0;
			window = arg;
			init_interface(0);
		} else if (arg is Gui)
		{
			window = new WidgetWindow(arg, NULL, 0, 0);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			window = new WidgetWindow(engine.getGui(), arg, 0, 0);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = window;
	}

	Window(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			window = new WidgetWindow(arg1, arg2, 0, 0);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			window = new WidgetWindow(engine.getGui(), arg1, arg2, 0);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = window;
	}

	Window(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, string, int)
		{
			window = new WidgetWindow(arg1, arg2, arg3, 0);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int, int)
		{
			window = new WidgetWindow(engine.getGui(), arg1, arg2, arg3);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = window;
	}

	Window(int arg1, int arg2, int arg3, int arg4)
	{
		if (arg1 is Gui) // (Gui, string, int, int)
		{
			window = new WidgetWindow(arg1, arg2, arg3, arg4);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int, int, int)
		{
			window = new WidgetWindow(engine.getGui(), arg1, arg2, arg3);
			init_interface(arg4);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = window;
	}

	Window(Gui gui, string str, int x, int y, int flags)
	{
		window = new WidgetWindow(gui, str, x, y);
		init_interface(flags);
		widget = window;
	}

	~Window()
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
	void setMoveable(int moveable) { window.setMoveable(moveable); }
	int isMoveable() { return window.isMoveable(); }

	// sizeable flag
	void setSizeable(int sizeable) { window.setSizeable(sizeable); }
	int isSizeable() { return window.isSizeable(); }

	// color multiplier
	void setColor(vec4 color) { window.setColor(color); }
	vec4 getColor() { return window.getColor(); }

	// space size
	void setSpace(int x, int y) { window.setSpace(x, y); }
	int getSpaceX() { return window.getSpaceX(); }
	int getSpaceY() { return window.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { window.setPadding(l, r, t, b); }
	int getPaddingLeft() { return window.getPaddingLeft(); }
	int getPaddingRight() { return window.getPaddingRight(); }
	int getPaddingTop() { return window.getPaddingTop(); }
	int getPaddingBottom() { return window.getPaddingBottom(); }

	// text
	void setText(string str) { window.setText(str); }
	string getText() { return window.getText(); }

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_WINDOW_H__ */
