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


#ifndef __UNIGINE_WIDGET_LABEL_H__
#define __UNIGINE_WIDGET_LABEL_H__

namespace Unigine::Widgets
{
class Label : Widget
{
	WidgetLabel label;

	// constructor
	Label()
	{
		label = new WidgetLabel(engine.getGui(), NULL);
		widget = label;
	}

	Label(int arg)
	{
		if (arg is WidgetLabel)
		{
			owner = 0;
			label = arg;
		} else if (arg is Gui)
			label = new WidgetLabel(arg, NULL);
		else if (is_null(arg) || is_string(arg))
			label = new WidgetLabel(engine.getGui(), arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = label;
	}

	Label(Gui gui, string str)
	{
		label = new WidgetLabel(gui, str);
		widget = label;
	}

	// text
	void setTextAlign(int align) { label.setTextAlign(align); }
	int getTextAlign() { return label.getTextAlign(); }
	void setText(string str) { label.setText(str); }
	string getText() { return label.getText(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LABEL_H__ */
