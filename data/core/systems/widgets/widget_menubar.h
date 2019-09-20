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


#ifndef __UNIGINE_WIDGET_MENUBAR_H__
#define __UNIGINE_WIDGET_MENUBAR_H__

#include <core/systems/widgets/widget_menubox.h>

namespace Unigine::Widgets
{
class MenuBar : Widget
{
	WidgetMenuBar menubar;

	// constructor
	MenuBar()
	{
		menubar = new WidgetMenuBar(engine.getGui(), 0, 0);
		widget = menubar;
	}

	MenuBar(int arg)
	{
		if (arg is WidgetMenuBar)
		{
			owner = 0;
			menubar = arg;
		} else if (arg is Gui)
			menubar = new WidgetMenuBar(arg, 0, 0);
		else if (is_int(arg))
			menubar = new WidgetMenuBar(engine.getGui(), arg, 0);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = menubar;
	}

	MenuBar(int arg1, int arg2)
	{
		if (arg1 is Gui)
			menubar = new WidgetMenuBar(arg1, arg2, 0);
		else if (is_int(arg1))
			menubar = new WidgetMenuBar(engine.getGui(), arg1, arg2);
		else
			throw("Unknown type of argument %s\n", typeof(arg1));

		widget = menubar;
	}

	MenuBar(Gui gui, int x, int y)
	{
		menubar = new WidgetMenuBar(gui, x, y);
		widget = menubar;
	}
	// space size
	void setSpace(int x, int y) { menubar.setSpace(x, y); }
	int getSpaceX() { return menubar.getSpaceX(); }
	int getSpaceY() { return menubar.getSpaceY(); }

	// clear
	void clear() { menubar.clear(); }

	// items
	int addItem(string str, MenuBox menu = NULL) { return menubar.addItem(str, (menu != NULL) ? menu.menubox : NULL); }
	void removeItem(int item) { menubar.removeItem(item); }
	int getNumItems() { return menubar.getNumItems(); }

	// item text
	void setItemText(int item, string str) { menubar.setItemText(item, str); }
	string getItemText(int item) { return menubar.getItemText(item); }

	// item data
	void setItemData(int item, string str) { menubar.setItemData(item, str); }
	string getItemData(int item) { return menubar.getItemData(item); }

	// item enabled flag
	void setItemEnabled(int item, int enable) { menubar.setItemEnabled(item, enable); }
	int isItemEnabled(int item) { return menubar.isItemEnabled(item); }

	// item space
	void setItemSpace(int item, int space) { menubar.setItemSpace(item, space); }
	int getItemSpace(int item) { return menubar.getItemSpace(item); }

	// item menu
	void setItemMenu(int item, MenuBox menu) { menubar.setItemMenu(item, (menu != NULL) ? menu.menubox : NULL); }
	MenuBox getItemMenu(int item) { return new MenuBox(menubar.getItemMenu(item)); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_MENUBAR_H__ */
