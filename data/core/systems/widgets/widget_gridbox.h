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


#ifndef __UNIGINE_WIDGET_GRIDBOX_H__
#define __UNIGINE_WIDGET_GRIDBOX_H__

namespace Unigine::Widgets
{
class GridBox : Widget
{
	WidgetGridBox gridbox;

	enum
	{
		DEFAULT_GRID_NUM = 2,
	};

	// constructor
	GridBox()
	{
		gridbox = new WidgetGridBox(engine.getGui(), DEFAULT_GRID_NUM, 0, 0);
		widget = gridbox;
	}

	GridBox(int arg)
	{
		if (arg is WidgetGridBox)
		{
			owner = 0;
			gridbox = arg;
		} else if (arg is Gui)
			gridbox = new WidgetGridBox(arg, DEFAULT_GRID_NUM, 0, 0);
		else if (is_int(arg))
			gridbox = new WidgetGridBox(engine.getGui(), arg, 0, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = gridbox;
	}

	GridBox(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, int)
			gridbox = new WidgetGridBox(arg1, arg2, 0, 0);
		else if (is_int(arg1)) // (int, int)
			gridbox = new WidgetGridBox(engine.getGui(), arg1, arg2, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = gridbox;
	}

	GridBox(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, int, int)
			gridbox = new WidgetGridBox(arg1, arg2, arg3, 0);
		else if (is_int(arg1)) // (int, int, int)
			gridbox = new WidgetGridBox(engine.getGui(), arg1, arg2, arg3);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = gridbox;
	}

	GridBox(Gui gui, int num, int x, int y)
	{
		gridbox = new WidgetGridBox(gui, num, x, y);
		widget = gridbox;
	}

	// space size
	void setSpace(int x, int y) { gridbox.setSpace(x, y); }
	int getSpaceX() { return gridbox.getSpaceX(); }
	int getSpaceY() { return gridbox.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { gridbox.setPadding(l, r, t, b); }
	int getPaddingLeft() { return gridbox.getPaddingLeft(); }
	int getPaddingRight() { return gridbox.getPaddingRight(); }
	int getPaddingTop() { return gridbox.getPaddingTop(); }
	int getPaddingBottom() { return gridbox.getPaddingBottom(); }

	// stencil flag
	void setStencil(int stencil) { gridbox.setStencil(stencil); }
	int getStencil() { return gridbox.getStencil(); }

	// color multiplier
	void setColor(vec4 color) { gridbox.setColor(color); }
	vec4 getColor() { return gridbox.getColor(); }

	// background flag
	void setBackground(int background) { gridbox.setBackground(background); }
	int getBackground() { return gridbox.getBackground(); }

	// columns
	void setNumColumns(int num) { gridbox.setNumColumns(num); }
	int getNumColumns() { return gridbox.getNumColumns(); }

	// column radio
	void setColumnRatio(int num, int radio) { gridbox.setColumnRatio(num, radio); }
	int getColumnRatio(int num) { return gridbox.getColumnRatio(num); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_GRIDBOX_H__ */
