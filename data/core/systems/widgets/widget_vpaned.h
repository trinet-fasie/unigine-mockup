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


#ifndef __UNIGINE_WIDGET_VPANED_H__
#define __UNIGINE_WIDGET_VPANED_H__

namespace Unigine::Widgets
{
class VPaned : Widget
{
	WidgetVPaned vpaned;

	// constructor
	VPaned()
	{
		vpaned = new WidgetVPaned(engine.getGui());
		widget = vpaned;
	}

	VPaned(int arg)
	{
		if (arg is WidgetVPaned)
		{
			owner = 0;
			vpaned = arg;
		} else if (arg is Gui)
			vpaned = new WidgetVPaned(arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = vpaned;
	}

	// value
	void setValue(int value) { vpaned.setValue(value); }
	int getValue() { return vpaned.getValue(); }

	// fixed
	void setFixed(int fixed) { vpaned.setFixed(fixed); }
	int getFixed() { return vpaned.getFixed(); }

	// first child size
	void setFirstSize(int size) { vpaned.setFirstSize(size); }
	int getFirstSize() { return vpaned.getFirstSize(); }

	// second child size
	void setSecondSize(int size) { vpaned.setSecondSize(size); }
	int getSecondSize() { return vpaned.getSecondSize(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_VPANED_H__ */
