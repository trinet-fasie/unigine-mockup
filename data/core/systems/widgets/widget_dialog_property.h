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


#ifndef __UNIGINE_WIDGET_DIALOG_PROPERTY_H__
#define __UNIGINE_WIDGET_DIALOG_PROPERTY_H__

#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_dialog.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_scrollbox.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_vbox.h>

namespace Unigine::Widgets
{
enum
{
	DIALOG_PROPERTY_WIDTH = 480,  // dialog width
	DIALOG_PROPERTY_HEIGHT = 320, // dialog height
};

class DialogProperty : Dialog
{
	Button ok_b;		   // ok button
	EditLine name_el;	  // name editline
	TreeBox properties_tb; // properties treebox

	// constructor
	DialogProperty()
		: Dialog(engine.getGui(), NULL)
	{
		init_widget(engine.getGui());
	}

	DialogProperty(int arg)
		: Dialog(arg)
	{
		if (arg is Gui)
			init_widget(arg);
		else
			init_widget(engine.getGui());
	}

	DialogProperty(Gui gui, string str)
		: Dialog(gui, str)
	{
		init_widget(gui);
	}

	void setProperty(string name)
	{
		// update properties
		properties_tb.setCallbackEnabled(CHANGED, 0);
		update_properties();
		properties_tb.setCallbackEnabled(CHANGED, 1);

		name_el.setText(name);
	}

	string getProperty()
	{
		return properties_tb.getCurrentItemText();
	}

	void update_properties()
	{
		string properties[0];
		forloop(int i = 0; engine.properties.getNumProperties())
		{
			Property property = engine.properties.getProperty(i);
			if (property.isHidden() == 1 || strlen(property.getName()) == 0)
				continue;
			properties.append(property.getName());
		}
		properties.sort();

		string items[];
		properties_tb.setCallbackEnabled(CHANGED, 0);
		properties_tb.clear();
		foreach (string name; properties)
		{
			items.append(name, properties_tb.addItem(name));
		}
		properties_tb.setCallbackEnabled(CHANGED, 1);

		foreach (string name; properties)
		{
			Property property = engine.properties.findProperty(name);
			Property parent = property.getParent();
			if (parent == NULL || properties.find(parent.getName()) == -1)
				continue;
			properties_tb.setItemParent(items[name], items[parent.getName()]);
		}
	}

	void name_changed()
	{
		name_el.setCallbackEnabled(CHANGED, 0);
		properties_tb.setCallbackEnabled(CHANGED, 0);
		int length = properties_tb.selectByName(name_el.getText());
		name_el.setText(substr(name_el.getText(), 0, length));
		name_el.setCallbackEnabled(CHANGED, 1);
		properties_tb.setCallbackEnabled(CHANGED, 1);
		ok_b.setEnabled((properties_tb.getCurrentItem() != -1));
	}

	void properties_changed()
	{
		name_el.setCallbackEnabled(CHANGED, 0);
		name_el.setText(properties_tb.getCurrentItemText());
		name_el.setCallbackEnabled(CHANGED, 1);
		ok_b.setEnabled((properties_tb.getCurrentItem() != -1));
	}

	void ok_clicked()
	{
		ok_b.runCallback(CLICKED);
	}

	void callback_redirector(DialogProperty dialog, string name)
	{
		dialog.call(name);
	}

	//
	void init_widget(Gui gui)
	{
		// ok button
		ok_b = getOkButton();

		// grid box
		VBox vbox = new VBox(gui, 0, 4);
		GridBox gridbox = new GridBox(gui, 2, 4, 4);
		vbox.addChild(gridbox, ALIGN_EXPAND);
		addChild(vbox);

		// property name
		name_el = new EditLine(gui);
		name_el.setCallback(CHANGED, functionid(callback_redirector), this, functionid(name_changed));
		name_el.setCallback(PRESSED, functionid(callback_redirector), this, functionid(ok_clicked));
		gridbox.addChild(new Label(gui, TR("Name:")), ALIGN_RIGHT);
		gridbox.addChild(name_el, ALIGN_EXPAND);

		// scrollbox
		ScrollBox scrollbox = new ScrollBox(gui);
		scrollbox.setWidth(DIALOG_PROPERTY_WIDTH);
		scrollbox.setHeight(DIALOG_PROPERTY_HEIGHT);
		addChild(scrollbox, ALIGN_EXPAND);

		// properties treebox
		properties_tb = new TreeBox(gui);
		properties_tb.setCallback(CHANGED, functionid(callback_redirector), this, functionid(properties_changed));
		properties_tb.setCallback(DOUBLE_CLICKED, functionid(callback_redirector), this, functionid(ok_clicked));
		scrollbox.addChild(properties_tb, ALIGN_EXPAND);
	}
};

int dialogPropertyGui(Gui gui, string str, string &name)
{
	DialogProperty dialog;

	// create dialog
	if (dialog == NULL)
	{
		dialog = new DialogProperty(gui, str);
		dialog.setSizeable(1);
	}

	// dialog property
	dialog.setText(str);
	dialog.setProperty(name);

	// show dialog
	dialog.arrange();
	addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);
	dialog.setPermanentFocus();

	// wait dialog
	while (dialog.isDone() == 0)
		wait;

	// dialog result
	if (dialog.isOkClicked())
		name = dialog.getProperty();

	// hide dialog
	removeChild(dialog);

	return dialog.isOkClicked();
}

int dialogProperty(string str, string &name)
{
	return dialogPropertyGui(engine.getGui(), str, name);
}

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_PROPERTY_H__ */
