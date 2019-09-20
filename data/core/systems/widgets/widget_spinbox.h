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


#ifndef __UNIGINE_WIDGET_SPINBOX_H__
#define __UNIGINE_WIDGET_SPINBOX_H__

namespace Unigine::Widgets
{
class SpinBox : Widget
{
	enum
	{
		DEFAULT_MIN = 0,
		DEFAULT_MAX = 100,
		DEFAULT_VALUE = 0,
	};

	WidgetSpinBox spinbox;

	// constructor
	SpinBox()
	{
		spinbox = new WidgetSpinBox(engine.getGui(), DEFAULT_MIN, DEFAULT_MAX, DEFAULT_VALUE);
		widget = spinbox;
	}

	SpinBox(int arg)
	{
		if (arg is WidgetSpinBox)
		{
			owner = 0;
			spinbox = arg;
		} else if (arg is Gui)
			spinbox = new WidgetSpinBox(arg, DEFAULT_MIN, DEFAULT_MAX, DEFAULT_VALUE);
		else if (is_int(arg))
			spinbox = new WidgetSpinBox(engine.getGui(), arg, DEFAULT_MAX, DEFAULT_VALUE);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = spinbox;
	}

	SpinBox(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, int)
			spinbox = new WidgetSpinBox(arg1, arg2, DEFAULT_MAX, DEFAULT_VALUE);
		else if (is_int(arg1)) // (int, int)
			spinbox = new WidgetSpinBox(engine.getGui(), arg1, arg2, DEFAULT_VALUE);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = spinbox;
	}

	SpinBox(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, int, int)
			spinbox = new WidgetSpinBox(arg1, arg2, arg3, DEFAULT_VALUE);
		else if (is_int(arg1)) // (int, int, int)
			spinbox = new WidgetSpinBox(engine.getGui(), arg1, arg2, arg3);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = spinbox;
	}

	SpinBox(Gui gui, int min, int max, int value)
	{
		spinbox = new WidgetSpinBox(gui, min, max, value);
		widget = spinbox;
	}

	// minimum value
	void setMinValue(int value) { spinbox.setMinValue(value); }
	int getMinValue() { return spinbox.getMinValue(); }

	// maximum value
	void setMaxValue(int value) { spinbox.setMaxValue(value); }
	int getMaxValue() { return spinbox.getMaxValue(); }

	// value
	void setValue(int value) { spinbox.setValue(value); }
	int getValue() { return spinbox.getValue(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SPINBOX_H__ */
