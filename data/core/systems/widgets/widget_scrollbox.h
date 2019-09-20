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


#ifndef __UNIGINE_WIDGET_SCROLLBOX_H__
#define __UNIGINE_WIDGET_SCROLLBOX_H__

namespace Unigine::Widgets
{
class ScrollBox : Widget
{
	WidgetScrollBox scrollbox;

	// constructor
	ScrollBox()
	{
		scrollbox = new WidgetScrollBox(engine.getGui(), 0, 0);
		widget = scrollbox;
	}

	ScrollBox(int arg)
	{
		if (arg is WidgetScrollBox)
		{
			owner = 0;
			scrollbox = arg;
		} else if (arg is Gui)
			scrollbox = new WidgetScrollBox(arg, 0, 0);
		else if (is_int(arg))
			scrollbox = new WidgetScrollBox(engine.getGui(), arg, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = scrollbox;
	}

	ScrollBox(int arg1, int arg2)
	{
		if (arg1 is Gui)
			scrollbox = new WidgetScrollBox(arg1, arg2, 0);
		else if (is_int(arg1))
			scrollbox = new WidgetScrollBox(engine.getGui(), arg1, arg2);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = scrollbox;
	}

	ScrollBox(Gui gui, int x, int y)
	{
		scrollbox = new WidgetScrollBox(gui, x, y);
		widget = scrollbox;
	}

	// space size
	void setSpace(int x, int y) { scrollbox.setSpace(x, y); }
	int getSpaceX() { return scrollbox.getSpaceX(); }
	int getSpaceY() { return scrollbox.getSpaceY(); }

	// padding size
	void setPadding(int l, int r, int t, int b) { scrollbox.setPadding(l, r, t, b); }
	int getPaddingLeft() { return scrollbox.getPaddingLeft(); }
	int getPaddingRight() { return scrollbox.getPaddingRight(); }
	int getPaddingTop() { return scrollbox.getPaddingTop(); }
	int getPaddingBottom() { return scrollbox.getPaddingBottom(); }

	// border flag
	void setBorder(int border) { scrollbox.setBorder(border); }
	int getBorder() { return scrollbox.getBorder(); }

	// scroll scale
	int getScrollScale() { return scrollbox.getScrollScale(); }

	// vertical scroll
	void setVScrollEnabled(int enable) { scrollbox.setVScrollEnabled(enable); }
	int isVScrollEnabled() { return scrollbox.isVScrollEnabled(); }
	void setVScrollHidden(int hide) { scrollbox.setVScrollHidden(hide); }
	int isVScrollHidden() { return scrollbox.isVScrollHidden(); }
	int getVScrollObjectSize() { return scrollbox.getVScrollObjectSize(); }
	int getVScrollFrameSize() { return scrollbox.getVScrollFrameSize(); }
	int getVScrollStepSize() { return scrollbox.getVScrollStepSize(); }
	void setVScrollValue(int value) { scrollbox.setVScrollValue(value); }
	int getVScrollValue() { return scrollbox.getVScrollValue(); }

	// horizontal scroll
	void setHScrollEnabled(int enable) { scrollbox.setHScrollEnabled(enable); }
	int isHScrollEnabled() { return scrollbox.isHScrollEnabled(); }
	void setHScrollHidden(int hide) { scrollbox.setHScrollHidden(hide); }
	int isHScrollHidden() { return scrollbox.isHScrollHidden(); }
	int getHScrollObjectSize() { return scrollbox.getHScrollObjectSize(); }
	int getHScrollFrameSize() { return scrollbox.getHScrollFrameSize(); }
	int getHScrollStepSize() { return scrollbox.getHScrollStepSize(); }
	void setHScrollValue(int value) { scrollbox.setHScrollValue(value); }
	int getHScrollValue() { return scrollbox.getHScrollValue(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SCROLLBOX_H__ */
