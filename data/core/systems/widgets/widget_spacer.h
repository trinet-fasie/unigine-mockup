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


#ifndef __UNIGINE_WIDGET_SPACER_H__
#define __UNIGINE_WIDGET_SPACER_H__

namespace Unigine::Widgets
{
class Spacer : Widget
{
	WidgetSpacer spacer;

	// constructor
	Spacer()
	{
		spacer = new WidgetSpacer(engine.getGui());
		widget = spacer;
	}

	Spacer(int arg)
	{
		if (arg is WidgetSpacer)
		{
			owner = 0;
			spacer = arg;
		} else if (arg is Gui)
			spacer = new WidgetSpacer(arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = spacer;
	}

	// orientation
	void setOrientation(int orientation) { spacer.setOrientation(orientation); }
	int getOrientation() { return spacer.getOrientation(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SPACER_H__ */
