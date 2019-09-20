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


#ifndef __UNIGINE_TRACKER_EDITOR_DIALOG_SETTINGS_H__
#define __UNIGINE_TRACKER_EDITOR_DIALOG_SETTINGS_H__

namespace Unigine::Tracker
{
class TrackerEditorDialogSettings : Unigine::Widgets::Dialog
{
	VBox vbox;		 // vbox
	GridBox gridbox; // gridbox

	EditLine min_time_el;  // minimum time editline
	EditLine max_time_el;  // maximum time editline
	EditLine unit_time_el; // unit time editline

	float min_time;  // minimum time
	float max_time;  // maximum time
	float unit_time; // unit time

	TrackerEditorDialogSettings(Gui gui, Tracker tracker, string str)
		: Dialog(gui, str)
	{
		vbox = new VBox(gui);
		addChild(vbox);

		gridbox = new GridBox(gui, 2, 4, 4);
		vbox.addChild(gridbox, ALIGN_EXPAND);

		// minimum time
		min_time_el = new EditLine(gui);
		min_time_el.setCallback(PRESSED, functionid(callback_redirector), this, "min_time_pressed");
		min_time_el.setCallback(FOCUS_OUT, functionid(callback_redirector), this, "min_time_pressed");
		gridbox.addChild(new Label(gui, TR("Min time:")), ALIGN_RIGHT);
		gridbox.addChild(min_time_el, ALIGN_EXPAND);

		// maximum time
		max_time_el = new EditLine(gui);
		max_time_el.setCallback(PRESSED, functionid(callback_redirector), this, "max_time_pressed");
		max_time_el.setCallback(FOCUS_OUT, functionid(callback_redirector), this, "max_time_pressed");
		gridbox.addChild(new Label(gui, TR("Max time:")), ALIGN_RIGHT);
		gridbox.addChild(max_time_el, ALIGN_EXPAND);

		// unit time
		unit_time_el = new EditLine(gui);
		unit_time_el.setCallback(PRESSED, functionid(callback_redirector), this, "unit_time_pressed");
		unit_time_el.setCallback(FOCUS_OUT, functionid(callback_redirector), this, "unit_time_pressed");
		gridbox.addChild(new Label(gui, TR("Unit time:")), ALIGN_RIGHT);
		gridbox.addChild(unit_time_el, ALIGN_EXPAND);
	}

	~TrackerEditorDialogSettings()
	{
		delete vbox;
		delete gridbox;

		delete min_time_el;
		delete max_time_el;
		delete unit_time_el;
	}

	void setMinTime(float time)
	{
		min_time_el.setText(widgetFormat(time));
		min_time = time;
	}

	float getMinTime()
	{
		return min_time;
	}

	void setMaxTime(float time)
	{
		max_time_el.setText(widgetFormat(time));
		max_time = time;
	}

	float getMaxTime()
	{
		return max_time;
	}

	void setUnitTime(float time)
	{
		unit_time_el.setText(widgetFormat(time));
		unit_time = time;
	}

	float getUnitTime()
	{
		return unit_time;
	}

	void min_time_pressed()
	{
		min_time = min(float(min_time_el.getText()), max_time);
		min_time_el.setText(widgetFormat(min_time));
	}

	void max_time_pressed()
	{
		max_time = max(float(max_time_el.getText()), min_time);
		max_time_el.setText(widgetFormat(max_time));
	}

	void unit_time_pressed()
	{
		unit_time = max(float(unit_time_el.getText()), EPSILON);
		unit_time_el.setText(widgetFormat(unit_time));
	}

	void callback_redirector(TrackerEditorDialogSettings dialog, string name)
	{
		dialog.call(name);
	}
};

} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_DIALOG_SETTINGS_H__ */
