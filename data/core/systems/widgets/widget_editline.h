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


#ifndef __UNIGINE_WIDGET_EDITLINE_H__
#define __UNIGINE_WIDGET_EDITLINE_H__

namespace Unigine::Widgets
{
class EditLine : Widget
{
	WidgetEditLine editline;

	// constructor
	EditLine()
	{
		editline = new WidgetEditLine(engine.getGui(), NULL);
		widget = editline;
	}

	EditLine(int arg)
	{
		if (arg is WidgetEditLine)
		{
			owner = 0;
			editline = arg;
		} else if (arg is Gui)
			editline = new WidgetEditLine(arg, NULL);
		else if (is_null(arg) || is_string(arg))
			editline = new WidgetEditLine(engine.getGui(), arg);
		else
			throw("Unknown type of argument %s\n", typeof(arg));

		widget = editline;
	}

	EditLine(Gui gui, string str)
	{
		editline = new WidgetEditLine(gui, str);
		widget = editline;
	}

	// editable flag
	void setEditable(int editable) { editline.setEditable(editable); }
	int isEditable() { return editline.isEditable(); }

	// password mode
	void setPassword(int password) { editline.setPassword(password); }
	int isPassword() { return editline.isPassword(); }

	// text validator
	void setValidator(int validator) { editline.setValidator(validator); }
	int getValidator() { return editline.getValidator(); }

	// background flag
	void setBackground(int background) { editline.setBackground(background); }
	int getBackground() { return editline.getBackground(); }

	// capacity
	void setCapacity(int capacity) { editline.setCapacity(capacity); }
	int getCapacity() { return editline.getCapacity(); }

	// cursor position
	void setCursor(int position) { editline.setCursor(position); }
	int getCursor() { return editline.getCursor(); }

	// selection position
	void setSelection(int position) { editline.setSelection(position); }
	int getSelection() { return editline.getSelection(); }

	// selection text
	string getSelectionText() { return editline.getSelectionText(); }
	void clearSelectionText() { editline.clearSelectionText(); }

	// text
	void setText(string str) { editline.setText(str); }
	string getText() { return editline.getText(); }
};

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_EDITLINE_H__ */
