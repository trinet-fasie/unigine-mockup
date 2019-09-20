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


#ifndef __UNIGINE_WIDGET_GROUPBOX_H__
#define __UNIGINE_WIDGET_GROUPBOX_H__

namespace Unigine::Widgets
{
class GroupBox : Widget
{
	WidgetGroupBox groupbox;

	// constructor
	GroupBox()
	{
		groupbox = new WidgetGroupBox(engine.getGui(), NULL, 0, 0);
		widget = groupbox;
	}

	GroupBox(int arg)
	{
		if (arg is WidgetGroupBox)
		{
			owner = 0;
			groupbox = arg;
		} else if (arg is Gui)
			groupbox = new WidgetGroupBox(arg, NULL, 0, 0);
		else if (is_null(arg) || is_string(arg))
			groupbox = new WidgetGroupBox(engine.getGui(), arg, 0, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = groupbox;
	}

	GroupBox(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
			groupbox = new WidgetGroupBox(arg1, arg2, 0, 0);
		else if (is_null(arg1) || is_string(arg1)) // (string, int)
			groupbox = new WidgetGroupBox(engine.getGui(), arg1, arg2, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = groupbox;
	}

	GroupBox(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, string, int)
			groupbox = new WidgetGroupBox(arg1, arg2, arg3, 0);
		else if (is_null(arg1) || is_string(arg1)) // (string, int, int)
			groupbox = new WidgetGroupBox(engine.getGui(), arg1, arg2, arg3);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = groupbox;
	}

	GroupBox(Gui gui, string str, int x, int y)
	{
		groupbox = new WidgetGroupBox(gui, str, x, y);
		widget = groupbox;
	}

	// space size
	void setSpace(int x, int y) { groupbox.setSpace(x, y); }
	int getSpaceX() { return groupbox.getSpaceX(); }
	int getSpaceY() { return groupbox.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { groupbox.setPadding(l, r, t, b); }
	int getPaddingLeft() { return groupbox.getPaddingLeft(); }
	int getPaddingRight() { return groupbox.getPaddingRight(); }
	int getPaddingTop() { return groupbox.getPaddingTop(); }
	int getPaddingBottom() { return groupbox.getPaddingBottom(); }

	// border flag
	void setBorder(int border) { groupbox.setBorder(border); }
	int getBorder() { return groupbox.getBorder(); }

	// stencil flag
	void setStencil(int stencil) { groupbox.setStencil(stencil); }
	int getStencil() { return groupbox.getStencil(); }

	// color multiplier
	void setColor(vec4 color) { groupbox.setColor(color); }
	vec4 getColor() { return groupbox.getColor(); }

	// background flag
	void setBackground(int background) { groupbox.setBackground(background); }
	int getBackground() { return groupbox.getBackground(); }

	// text
	void setText(string str) { groupbox.setText(str); }
	string getText() { return groupbox.getText(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_GROUPBOX_H__ */
