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


#ifndef __UNIGINE_WIDGET_HPANED_H__
#define __UNIGINE_WIDGET_HPANED_H__

namespace Unigine::Widgets
{
class HPaned : Widget
{
	WidgetHPaned hpaned;

	// constructor
	HPaned()
	{
		hpaned = new WidgetHPaned(engine.getGui());
		widget = hpaned;
	}

	HPaned(int arg)
	{
		if (arg is WidgetHPaned)
		{
			owner = 0;
			hpaned = arg;
		} else if (arg is Gui)
			hpaned = new WidgetHPaned(arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = hpaned;
	}

	// value
	void setValue(int value) { hpaned.setValue(value); }
	int getValue() { return hpaned.getValue(); }

	// fixed
	void setFixed(int fixed) { hpaned.setFixed(fixed); }
	int getFixed() { return hpaned.getFixed(); }

	// first child size
	void setFirstSize(int size) { hpaned.setFirstSize(size); }
	int getFirstSize() { return hpaned.getFirstSize(); }

	// second child size
	void setSecondSize(int size) { hpaned.setSecondSize(size); }
	int getSecondSize() { return hpaned.getSecondSize(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_HPANED_H__ */
