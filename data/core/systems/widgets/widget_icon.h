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


#ifndef __UNIGINE_WIDGET_ICON_H__
#define __UNIGINE_WIDGET_ICON_H__

namespace Unigine::Widgets
{
class Icon : Widget
{
	WidgetIcon icon;

	// constructor
	Icon()
	{
		icon = new WidgetIcon(engine.getGui(), NULL, 0, 0);
		widget = icon;
	}

	Icon(int arg)
	{
		if (arg is WidgetIcon)
		{
			owner = 0;
			icon = arg;
		} else if (arg is Gui)
			icon = new WidgetIcon(arg, NULL, 0, 0);
		else if (is_null(arg) || is_string(arg))
			icon = new WidgetIcon(engine.getGui(), arg, 0, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = icon;
	}

	Icon(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
			icon = new WidgetIcon(arg1, arg2, 0, 0);
		else if (is_null(arg1) || is_string(arg1)) // (string, int)
			icon = new WidgetIcon(engine.getGui(), arg1, arg2, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = icon;
	}

	Icon(int arg1, int arg2, int arg3)
	{
		if (arg1 is Gui) // (Gui, string, int)
			icon = new WidgetIcon(arg1, arg2, arg3, 0);
		else if (is_null(arg1) || is_string(arg1)) // (string, int, int)
			icon = new WidgetIcon(engine.getGui(), arg1, arg2, arg3);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = icon;
	}

	Icon(Gui gui, string str, int width, int height)
	{
		icon = new WidgetIcon(gui, str, width, height);
		widget = icon;
	}

	// toggleable flag
	void setToggleable(int toggleable) { icon.setToggleable(toggleable); }
	int isToggleable() { return icon.isToggleable(); }

	// toggle status
	void setToggled(int toggled) { icon.setToggled(toggled); }
	int isToggled() { return icon.isToggled(); }

	// texture
	void setImage(Image image) { icon.setImage(image); }
	Image getImage() { return icon.getImage(); }
	void setTexture(string name) { icon.setTexture(name); }
	string getTexture() { return icon.getTexture(); }

	// text
	void setTextAlign(int align) { icon.setTextAlign(align); }
	int getTextAlign() { return icon.getTextAlign(); }
	void setText(string str) { icon.setText(str); }
	string getText() { return icon.getText(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_ICON_H__ */
