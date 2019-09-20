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


#ifndef __UNIGINE_WIDGET_DIALOG_MATERIAL_H__
#define __UNIGINE_WIDGET_DIALOG_MATERIAL_H__

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
	DIALOG_MATERIAL_WIDTH = 480,  // dialog width
	DIALOG_MATERIAL_HEIGHT = 320, // dialog height
};

class DialogMaterial : Dialog
{
	Button ok_b;		   // ok button
	EditLine name_el;	  // name editline
	TreeBox materials_tb;  // materials treebox

	// constructor
	DialogMaterial()
		: Dialog(engine.getGui(), NULL)
	{
		init_widget(engine.getGui());
	}

	DialogMaterial(int arg)
		: Dialog(arg)
	{
		if (arg is Gui)
			init_widget(arg);
		else
			init_widget(engine.getGui());
	}

	DialogMaterial(Gui gui, string str)
		: Dialog(gui, str)
	{
		init_widget(gui);
	}

	void setMaterial(string name)
	{
		// update materials
		materials_tb.setCallbackEnabled(CHANGED, 0);
		update_materials();
		materials_tb.setCallbackEnabled(CHANGED, 1);

		name_el.setText(name);
	}

	string getMaterial()
	{
		return materials_tb.getCurrentItemText();
	}

	void update_materials()
	{
		string materials[0];
		forloop(int i = 0; engine.materials.getNumMaterials())
		{
			Material material = engine.materials.getMaterial(i);
			if (material.isHidden() == 1 || strlen(material.getName()) == 0)
				continue;
			materials.append(material.getName());
		}
		
		materials.sort();

		string items[];
		materials_tb.setCallbackEnabled(CHANGED, 0);
		materials_tb.clear();
		foreach (string name; materials)
		{
			items.append(name, materials_tb.addItem(name));
		}
		materials_tb.setCallbackEnabled(CHANGED, 1);

		foreach (string name; materials)
		{
			Material material = engine.materials.findMaterial(name);
			Material parent = material.getParent();
			if (parent == NULL || materials.find(parent.getName()) == -1)
				continue;
			materials_tb.setItemParent(items[name], items[parent.getName()]);
		}
	}

	void name_changed()
	{
		name_el.setCallbackEnabled(CHANGED, 0);
		materials_tb.setCallbackEnabled(CHANGED, 0);
		int length = materials_tb.selectByName(name_el.getText());
		name_el.setText(substr(name_el.getText(), 0, length));
		name_el.setCallbackEnabled(CHANGED, 1);
		materials_tb.setCallbackEnabled(CHANGED, 1);
		ok_b.setEnabled((materials_tb.getCurrentItem() != -1));
	}

	void materials_changed()
	{
		name_el.setCallbackEnabled(CHANGED, 0);
		name_el.setText(materials_tb.getCurrentItemText());
		name_el.setCallbackEnabled(CHANGED, 1);
		ok_b.setEnabled((materials_tb.getCurrentItem() != -1));
	}

	void ok_clicked()
	{
		ok_b.runCallback(CLICKED);
	}

	void callback_redirector(DialogMaterial dialog, string name)
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

		// material name
		name_el = new EditLine(gui);
		name_el.setCallback(CHANGED, functionid(callback_redirector), this, functionid(name_changed));
		name_el.setCallback(PRESSED, functionid(callback_redirector), this, functionid(ok_clicked));
		gridbox.addChild(new Label(gui, TR("Name:")), ALIGN_RIGHT);
		gridbox.addChild(name_el, ALIGN_EXPAND);

		// scrollbox
		ScrollBox scrollbox = new ScrollBox(gui);
		scrollbox.setWidth(DIALOG_MATERIAL_WIDTH);
		scrollbox.setHeight(DIALOG_MATERIAL_HEIGHT);
		addChild(scrollbox, ALIGN_EXPAND);

		// materials treebox
		materials_tb = new TreeBox(gui);
		materials_tb.setCallback(CHANGED, functionid(callback_redirector), this, functionid(materials_changed));
		materials_tb.setCallback(DOUBLE_CLICKED, functionid(callback_redirector), this, functionid(ok_clicked));
		scrollbox.addChild(materials_tb, ALIGN_EXPAND);
	}
};

int dialogMaterialGui(Gui gui, string str, string &name)
{
	DialogMaterial dialog;

	// create dialog
	if (dialog == NULL)
	{
		dialog = new DialogMaterial(gui, str);
		dialog.setSizeable(1);
	}

	// dialog material
	dialog.setText(str);
	dialog.setMaterial(name);

	// show dialog
	dialog.arrange();
	addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);
	dialog.setPermanentFocus();

	// wait dialog
	while (dialog.isDone() == 0)
		wait;

	// dialog result
	if (dialog.isOkClicked())
		name = dialog.getMaterial();

	// hide dialog
	removeChild(dialog);

	return dialog.isOkClicked();
}

int dialogMaterial(string str, string &name)
{
	return dialogMaterialGui(engine.getGui(), str, name);
}

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_MATERIAL_H__ */
