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


#ifndef __UNIGINE_WIDGET_SLIDER_H__
#define __UNIGINE_WIDGET_SLIDER_H__

namespace Unigine::Widgets
{
class Slider : Widget
{
	enum
	{
		DEFAULT_MIN = 0,
		DEFAULT_MAX = 100,
		DEFAULT_VALUE = 0,
	};

	WidgetSlider slider;

	// constructor
	Slider()
	{
		slider = new WidgetSlider(engine.getGui(), DEFAULT_MIN, DEFAULT_MAX, DEFAULT_VALUE);
		widget = slider;
	}

	Slider(int arg)
	{
		if (arg is WidgetSlider)
		{
			owner = 0;
			slider = arg;
		} else if (arg is Gui)
			slider = new WidgetSlider(arg, DEFAULT_MIN, DEFAULT_MAX, DEFAULT_VALUE);
		else if (is_int(arg))
			slider = new WidgetSlider(engine.getGui(), arg, DEFAULT_MAX, DEFAULT_VALUE);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = slider;
	}

	Slider(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, int)
			slider = new WidgetSlider(arg1, arg2, DEFAULT_MAX, DEFAULT_VALUE);
		else if (is_int(arg1)) // (int, int)
			slider = new WidgetSlider(engine.getGui(), arg1, arg2, DEFAULT_VALUE);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = slider;
	}

	Slider(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, int, int)
			slider = new WidgetSlider(arg1, arg2, arg3, DEFAULT_VALUE);
		else if (is_int(arg1)) // (int, int, int)
			slider = new WidgetSlider(engine.getGui(), arg1, arg2, arg3);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = slider;
	}

	Slider(Gui gui, int min, int max, int value)
	{
		slider = new WidgetSlider(gui, min, max, value);
		widget = slider;
	}

	// orientation
	void setOrientation(int orientation) { slider.setOrientation(orientation); }
	int getOrientation() { return slider.getOrientation(); }

	// minimum value
	void setMinValue(int value) { slider.setMinValue(value); }
	int getMinValue() { return slider.getMinValue(); }

	// maximum value
	void setMaxValue(int value) { slider.setMaxValue(value); }
	int getMaxValue() { return slider.getMaxValue(); }

	// value
	void setValue(int value) { slider.setValue(value); }
	int getValue() { return slider.getValue(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SLIDER_H__ */
