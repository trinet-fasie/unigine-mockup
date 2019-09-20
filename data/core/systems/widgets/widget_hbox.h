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


#ifndef __UNIGINE_WIDGET_HBOX_H__
#define __UNIGINE_WIDGET_HBOX_H__

namespace Unigine::Widgets
{
class HBox : Widget
{
	WidgetHBox hbox;

	// constructor
	HBox()
	{
		hbox = new WidgetHBox(engine.getGui(), 0, 0);
		widget = hbox;
	}

	HBox(int arg)
	{
		if (arg is WidgetHBox)
		{
			owner = 0;
			hbox = arg;
		} else if (arg is Gui)
			hbox = new WidgetHBox(arg, 0, 0);
		else if (is_int(arg))
			hbox = new WidgetHBox(engine.getGui(), arg, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = hbox;
	}

	HBox(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, int)
			hbox = new WidgetHBox(arg1, arg2, 0);
		else if (is_int(arg1)) // (int, int)
			hbox = new WidgetHBox(engine.getGui(), arg1, arg2);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = hbox;
	}

	HBox(Gui gui, int x, int y)
	{
		hbox = new WidgetHBox(gui, x, y);
		widget = hbox;
	}

	// space size
	void setSpace(int x, int y) { hbox.setSpace(x, y); }
	int getSpaceX() { return hbox.getSpaceX(); }
	int getSpaceY() { return hbox.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { hbox.setPadding(l, r, t, b); }
	int getPaddingLeft() { return hbox.getPaddingLeft(); }
	int getPaddingRight() { return hbox.getPaddingRight(); }
	int getPaddingTop() { return hbox.getPaddingTop(); }
	int getPaddingBottom() { return hbox.getPaddingBottom(); }

	// stencil flag
	void setStencil(int stencil) { hbox.setStencil(stencil); }
	int getStencil() { return hbox.getStencil(); }

	// color multiplier
	void setColor(vec4 color) { hbox.setColor(color); }
	vec4 getColor() { return hbox.getColor(); }

	// background flag
	void setBackground(int background) { hbox.setBackground(background); }
	int getBackground() { return hbox.getBackground(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_HBOX_H__ */
