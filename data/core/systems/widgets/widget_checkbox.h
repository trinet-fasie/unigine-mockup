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


#ifndef __UNIGINE_WIDGET_CHECKBOX_H__
#define __UNIGINE_WIDGET_CHECKBOX_H__

namespace Unigine::Widgets
{
class CheckBox : Widget
{
	WidgetCheckBox checkbox;

	// constructor
	CheckBox()
	{
		checkbox = new WidgetCheckBox(engine.getGui(), NULL);
		widget = checkbox;
	}

	CheckBox(int arg)
	{
		if (arg is WidgetCheckBox)
		{
			owner = 0;
			checkbox = arg;
		} else if (arg is Gui)
			checkbox = new WidgetCheckBox(arg, NULL);
		else if (is_null(arg) || is_string(arg))
			checkbox = new WidgetCheckBox(engine.getGui(), arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = checkbox;
	}

	CheckBox(Gui gui, string str)
	{
		checkbox = new WidgetCheckBox(gui, str);
		widget = checkbox;
	}

	// check status
	void setChecked(int checked) { checkbox.setChecked(checked); }
	int isChecked() { return checkbox.isChecked(); }

	// text
	void setText(string str) { checkbox.setText(str); }
	string getText() { return checkbox.getText(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_CHECKBOX_H__ */
