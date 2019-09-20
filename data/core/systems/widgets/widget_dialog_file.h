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


#ifndef __UNIGINE_WIDGET_DIALOG_FILE_H__
#define __UNIGINE_WIDGET_DIALOG_FILE_H__

#include <core/systems/widgets/widget_dialog.h>

namespace Unigine::Widgets
{
class DialogFile : Dialog
{
	WidgetDialogFile dialog_file;

	// constructor
	DialogFile()
	{
		dialog_file = new WidgetDialogFile(engine.getGui(), NULL);
		init_interface(0);
		dialog = dialog_file;
		widget = dialog;
	}

	DialogFile(int arg)
	{
		if (arg is Gui)
		{
			dialog_file = new WidgetDialogFile(arg, NULL);
			init_interface(0);
		} else if (is_null(arg) || is_string(arg))
		{
			dialog_file = new WidgetDialogFile(engine.getGui(), arg);
			init_interface(0);
		} else
			throw("Unknown type of argument %s\n", typeof(arg));

		dialog = dialog_file;
		widget = dialog;
	}

	DialogFile(int arg1, int arg2)
	{
		if (arg1 is Gui) // (Gui, string)
		{
			dialog_file = new WidgetDialogFile(arg1, arg2);
			init_interface(0);
		} else if (is_null(arg1) || is_string(arg1)) // (string, int)
		{
			dialog_file = new WidgetDialogFile(engine.getGui(), arg1);
			init_interface(arg2);
		} else
			throw("Unknown type of argument %s\n", typeof(arg1));

		dialog = dialog_file;
		widget = dialog;
	}

	DialogFile(Gui gui, string str, int flags)
	{
		dialog_file = new WidgetDialogFile(gui, str);
		init_interface(flags);
		dialog = dialog_file;
		widget = dialog;
	}

	// path
	void setPath(string str) { dialog_file.setPath(str); }
	string getPath() { return dialog_file.getPath(); }

	// file
	void setFile(string str) { dialog_file.setFile(str); }
	string getFile() { return dialog_file.getFile(); }

	// tabs
	void setTabs(string str) { dialog_file.setTabs(str); }
	string getTabs() { return dialog_file.getTabs(); }

	// filter
	void setFilter(string str) { dialog_file.setFilter(str); }
	string getFilter() { return dialog_file.getFilter(); }

	//
	void init_interface(int flags)
	{
#ifdef HAS_INTERFACE
		interface = new InterfaceWindow(flags);
#endif
	}
};

int dialogFileGui(Gui gui, string str, string filter, string &file)
{
	DialogFile dialog;

	// data path
	string path = engine.getDataPath();

	// config name
	string config = "unigine_widgets_dialog_file";

	// create dialog
	if (dialog == NULL)
	{
		dialog = new DialogFile(gui, str);
		if (engine.config.isExist(config))
			dialog.setTabs(engine.config.getString(config));
		dialog.setSizeable(1);
	}

	// dialog file
	dialog.setText(str);
	dialog.setFilter(filter);
	if (strlen(file))
		dialog.setFile(path + file);

	// show dialog
	dialog.arrange();
	addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);
	dialog.setPermanentFocus();

	// wait dialog
	while (dialog.isDone() == 0)
		wait;

	// dialog result
	if (dialog.isOkClicked())
		file = relname(path, dialog.getFile());

	// save config
	engine.config.setString(config, dialog.getTabs());

	// hide dialog
	removeChild(dialog);

	return dialog.isOkClicked();
}

int dialogFile(string str, string filter, string &file)
{
	return dialogFileGui(engine.getGui(), str, filter, file);
}

#define DECLARE_DIALOG_FILE(NAME, CONFIG, FILTER)                     \
	int dialogFile ## NAME ## Gui(Gui gui, string str, string &file)  \
	{                                                                 \
		DialogFile dialog;                                            \
		string path = engine.getDataPath();                           \
		if (dialog == NULL)                                           \
		{                                                             \
			dialog = new DialogFile(gui, str);                        \
			if (engine.config.isExist(CONFIG))                        \
				dialog.setTabs(engine.config.getString(CONFIG));      \
			dialog.setSizeable(1);                                    \
		}                                                             \
		dialog.setText(str);                                          \
		dialog.setFilter(FILTER);                                     \
		if (strlen(file))                                             \
			dialog.setFile(path + file);                              \
		dialog.arrange();                                             \
		addChild(dialog, ALIGN_OVERLAP | ALIGN_CENTER);               \
		dialog.setPermanentFocus();                                   \
		while (dialog.isDone() == 0)                                  \
			wait;                                                     \
		if (dialog.isOkClicked())                                     \
			file = replace(dialog.getFile(), path, "");               \
		engine.config.setString(CONFIG, dialog.getTabs());            \
		removeChild(dialog);                                          \
		return dialog.isOkClicked();                                  \
	}                                                                 \
	int dialogFile ## NAME(string str, string &file)                  \
	{                                                                 \
		return dialogFile ## NAME ## Gui(engine.getGui(), str, file); \
	}

DECLARE_DIALOG_FILE(Node, "unigine_widgets_dialog_file_node", ".node")
DECLARE_DIALOG_FILE(Mesh, "unigine_widgets_dialog_file_mesh", ".mesh.anim")
DECLARE_DIALOG_FILE(Image, "unigine_widgets_dialog_file_image", ".tga.jpg.png.psd.ppm.pgm.dds")
DECLARE_DIALOG_FILE(Sound, "unigine_widgets_dialog_file_sound", ".wav.oga.mp3")
DECLARE_DIALOG_FILE(Path, "unigine_widgets_dialog_file_path", ".path")

#undef DECLARE_DIALOG_FILE

} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_FILE_H__ */
