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


#ifndef __UNIGINE_SYSTEM_H__
#define __UNIGINE_SYSTEM_H__

//#define MENU_USER
//#define MOUSE_USER
//#define MOUSE_SOFT
//#define EXPORT_LOCALE
//#define EXPORT_EXTERN
//#define RENDER_SIMPLE

#ifndef VIDEO_FULLSCREEN
#define VIDEO_FULLSCREEN 1
#endif
#ifndef RENDER_OCCLUSION
#define RENDER_OCCLUSION 1
#endif
#ifndef SHOW_GPU
#define SHOW_GPU 1
#endif

#ifdef HAS_INTERFACE
#undef VIDEO_FULLSCREEN
#define VIDEO_FULLSCREEN 2
#endif

namespace System
{
string name;
string languages[0];
string locales[0];

string language_callback;
string language_world_callback;
string language_editor_callback;

UserInterface user_interface;

WidgetWindow main_w;
WidgetTabBox main_tb;
WidgetButton apply_b;

WidgetWindow event_w;

WidgetDialog message_d;
WidgetLabel message_l;

WidgetComboBox language_cb;

WidgetLabel gpu_l;

#ifdef HAS_INTERFACE
InterfaceWindow main_i;
InterfaceWindow event_i;
InterfaceWindow message_i;
#endif

string splash_4x3_name;
string splash_16x9_name;
int splash_threshold;
vec4 splash_transform;
vec4 splash_color;
string splash_text;

int mouse_user;
int toggle;
int enabled;
int clicked;
int language;

string tab_names[] = ("Video",
	"Render",
	"Interface",
	"Controls",
	"Plugins", );

string tab_prefix[] = ("System::Video::",
	"System::Render::",
	"System::Interface::",
	"System::Controls::",
	"System::Plugins::", );

int run_function(string name)
{
	foreach (string tab, i = 0; tab_names; i++)
	{
		if (tab == main_tb.getCurrentTabText() || engine.gui.translate(tab) == main_tb.getCurrentTabText())
			return call(tab_prefix[i] + name);
	}
	return 0;
}

int message_ret;

void message_ok_clicked()
{
	message_ret = 1;
#ifdef HAS_INTERFACE
	message_i.setWidget(NULL);
	message_i.setHidden(1);
#else
	engine.gui.removeChild(message_d);
#endif
}

void message_cancel_clicked()
{
	message_ret = -1;
#ifdef HAS_INTERFACE
	message_i.setWidget(NULL);
	message_i.setHidden(1);
#else
	engine.gui.removeChild(message_d);
#endif
}

int message(string title, string message)
{
	if (message_d == NULL)
	{
		log.error("System::message(): message dialog is NULL\n");
		log.message("%s\n%s\n", title, message);
		return;
	}

	message_d.setText(engine.gui.translate(title));
	message_l.setText(engine.gui.translate(message));

	WidgetButton ok_b = message_d.getOkButton();
	WidgetButton cancel_b = message_d.getCancelButton();
	ok_b.setCallback(GUI_CLICKED, "System::message_ok_clicked");
	cancel_b.setCallback(GUI_CLICKED, "System::message_cancel_clicked");

#ifdef HAS_INTERFACE
	if (message_i == NULL)
		message_i = new InterfaceWindow(0);
	message_i.setWidget(message_d);
	message_i.setCenterPosition();
	message_i.setHidden(0);
#else
	message_d.setOrder(125);
	engine.gui.addChild(message_d, GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
	message_d.setPermanentFocus();
#endif

	message_ret = 0;
	while (message_ret == 0 && message_d.isDone() == 0)
		wait 1;

	return (message_ret == 1);
}

void need_restart()
{
	message("Info", "You must restart Unigine to apply this settings");
}

void tab_changed()
{
	run_function("update");
}

void apply_clicked()
{
	run_function("apply");
	run_function("update");
	clicked = 1;
}

void quit_clicked()
{
	if (message("Confirm", "Quit from application?"))
	{
		engine.gui.removeChild(message_d);
		engine.console.run("quit");
	}
}

void update_language()
{
	language_cb.clear();
	foreach (string language; languages)
	{
		language_cb.addItem(engine.gui.translate(language));
	}
	language_cb.setCurrentItem(language);
}

void set_language(int l)
{
#ifndef EXPORT_LOCALE
	if (language == l)
		return;
	language = l;
	if (locales.check(language))
	{
		string tokens[2];
		engine.gui.clearDictionaries();
		if (strsplit(locales[language], ":", tokens) == 2)
			engine.gui.addDictionary(tokens[0], tokens[1]);
		else
			engine.gui.addDictionary(locales[language]);
		user_interface.updateWidgets();
		foreach (string tab; tab_prefix)
		{
			call(tab + "init");
			call(tab + "update");
		}
		if (language_cb != NULL)
			update_language();
		if (language_callback != NULL)
			call(language_callback);
		if (language_world_callback != NULL && engine.world.isLoaded())
			engine.world.call(language_world_callback);
		if (language_editor_callback != NULL && engine.editor.isLoaded())
			engine.editor.call(language_editor_callback);
	}
#endif
}

int get_language()
{
	return language;
}

void update_splash()
{
	float aspect = float(engine.app.getWidth()) / engine.app.getHeight();

	// system splash
	engine.splash.setSystem((aspect < 1.5f) ? splash_4x3_name : splash_16x9_name, splash_threshold);
	engine.splash.setSystemTransform(splash_transform);
	engine.splash.setSystemBackground(splash_color);
	engine.splash.setSystemText(splash_text);

	// world splash
	engine.splash.setWorld((aspect < 1.5f) ? splash_4x3_name : splash_16x9_name, splash_threshold);
	engine.splash.setWorldTransform(splash_transform);
	engine.splash.setWorldBackground(splash_color);
	engine.splash.setWorldText(splash_text);
}

void init(string name_, string languages_[], string locales_[])
{
	name = name_;
	languages.copy(languages_);
	locales.copy(locales_);
	mouse_user = 0;
	toggle = APP_KEY_ESC;
	language = -1;

	// splash parameters
	splash_4x3_name = "core/gui/splash.png";
	splash_16x9_name = "core/gui/splash.png";
	splash_transform = vec4(0.5f, 0.33f, 0.5f, 0.5f);
	splash_color = engine.gui.parseColor("000b11");
	splash_text = "<p align=center><font color=#606060 size=16><xy y=%100 dy=-20/>UNIGINE (c) 2005-2019</font></p>";
	splash_threshold = 16;

// export locale
#ifdef EXPORT_LOCALE
	if (locales.size())
	{
		string tokens[0];
		if (strsplit(locales[0], ":", tokens) == 2)
			engine.gui.saveDictionary(tokens[0], tokens[1]);
		else
			engine.gui.saveDictionary(locales[0]);
	}
#endif

// export extern
#ifdef EXPORT_EXTERN
	File file = new File();
	if (file.open("extern_info.h", "wb"))
	{
		file.puts(get_extern_info(EXPORT_EXTERN));
		file.close();
	}
	delete file;
#endif

	// initialize window
	user_interface = new UserInterface(engine.getGui(), name);
#ifndef HAS_INTERFACE
	engine.gui.addChild(main_w, GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
	engine.gui.removeChild(main_w);
#endif

	// set language
	set_language(engine.config.getInt("system_language", 0));

	// initialize tabs
	foreach (string tab; tab_prefix)
	{
		call(tab + "init");
		call(tab + "update");
	}

	// update language
	if (language_cb != NULL)
		update_language();

	// update splash
	update_splash();
}

void shutdown()
{
// remove widgets
#ifdef HAS_INTERFACE
	delete main_i;
	delete event_i;
	delete message_i;
#else
	engine.gui.removeChild(main_w);
#endif
	if (gpu_l != NULL)
		engine.gui.removeChild(gpu_l);

	// save config
	engine.config.setInt("system_language", language);
}

void show()
{
	if (enabled == 0)
	{
		enabled = 1;
#ifdef HAS_INTERFACE
		if (main_i == NULL)
		{
			main_i = new InterfaceWindow(0);
			main_i.setWidget(main_w);
			main_i.setCenterPosition();
		}
		main_i.setWidget(main_w);
		main_i.setHidden(0);
#else
		main_w.setHidden(0);
		main_w.setOrder(125);
		engine.gui.addChild(main_w, GUI_ALIGN_OVERLAP);
#endif
		main_w.setFocus();
		run_function("update");
	}
}

void hide()
{
	if (enabled == 1)
	{
		enabled = 0;
#ifdef HAS_INTERFACE
		if (main_i != NULL)
		{
			main_i.setWidget(NULL);
			main_i.setHidden(1);
		}
#else
		engine.gui.removeChild(main_w);
#endif
	}
#ifdef HAS_INTERFACE
	if (message_i != NULL)
	{
		message_i.setWidget(NULL);
		message_i.setHidden(1);
	}
#else
	engine.gui.removeChild(message_d);
#endif
}

void set_mouse_user(int enabled)
{
	mouse_user = enabled;
}

int is_mouse_user()
{
	return mouse_user;
}

void set_toggle(int t)
{
	toggle = t;
}

int get_toggle()
{
	return toggle;
}

int get_activity()
{
	return enabled;
}

#ifdef HAS_GPU_MONITOR

void show_gpu()
{
	log.message("show_gpu = %d\n", engine.config.getInt("show_gpu", SHOW_GPU));
}

void show_gpu(string show)
{
	engine.config.setInt("show_gpu", int(show));
}

#endif

void update()
{
	int init;
	float time;
	int mouse_x;
	int mouse_y;

	if (init == 0)
	{
		init = 1;
		time = clock();
		mouse_x = engine.app.getWidth() / 2;
		mouse_y = engine.app.getHeight() / 2;
	}

#ifdef MOUSE_USER

	// don't handle mouse
	
	// there are two ways to activate it: 
	//  1) -extern_define "MOUSE_USER"
	//  2) systemSetMouseUser(1);

#elif MOUSE_SOFT

	if (!engine.editor.isLoaded() && !mouse_user)
	{
		// hide gui mouse
		engine.gui.setMouseCursor(GUI_CURSOR_NONE);

		// switch mouse cursor
		if (mouse_x == engine.app.getMouseX() && mouse_y == engine.app.getMouseY())
		{
			if (clock() - time > 1.0f && engine.gui.getActivity() == 0)
				engine.app.setMouseShow(0);
			else
				engine.app.setMouseShow(1);
		} else
		{
			time = clock();
			mouse_x = engine.app.getMouseX();
			mouse_y = engine.app.getMouseY();
			engine.app.setMouseShow(1);
		}

		// engine controls
		if (engine.gui.getMouseButton() == APP_BUTTON_LEFT && engine.gui.getActivity() == 0)
			engine.controls.setMouseEnabled(1);
		else
			engine.controls.setMouseEnabled(0);
	}

#else

	if (!engine.editor.isLoaded() && !mouse_user)
	{
		// mouse status
		int mouse_status;
		if (engine.gui.getMouseButton() == APP_BUTTON_LEFT)
			mouse_status++;
		else
			mouse_status = 0;

		// hide system mouse
		engine.app.setMouseShow(0);

		// engine controls
		if (engine.controls.isMouseEnabled())
		{
			if (engine.app.clearKeyState(toggle) || engine.console.getActivity())
			{
				if (engine.app.getMouseGrab())
					engine.app.setMouse(mouse_x, mouse_y);
				engine.controls.setMouseEnabled(0);
				engine.gui.setMouseEnabled(1);
				engine.app.setMouseGrab(0);
			}
		} else if (engine.gui.getActivity() == 0 && mouse_status > 1)
		{
			mouse_x = engine.app.getMouseX();
			mouse_y = engine.app.getMouseY();
			engine.controls.setMouseEnabled(1);
			engine.gui.setMouseEnabled(0);
			engine.app.setMouseGrab(1);
		}
	}

#endif

#ifdef MENU_USER

// don't handle menu

#else

	// show/hide main window
	if (engine.console.getActivity() == 0 && engine.gui.getMouseGrab() == 0 &&
		engine.gui.getPermanentFocus() == NULL && engine.app.clearKeyState(toggle))
	{
		if (enabled == 0)
			show();
		else
			hide();
	}

#endif

	// update system
	if (enabled)
	{
		run_function("update_dynamic");
		apply_b.setEnabled(run_function("is_changed"));
	}

	// apply status
	if (clicked)
	{
		clicked++;
		if (apply_b.isEnabled() && engine.getError() != "")
		{
			engine.dialogError(engine.gui.translate("Can't apply settings"));
			clicked = 0;
		}
		if (clicked == 4)
			clicked = 0;
	}

	// key states
	int get_key_state(int state)
	{
		int key = engine.controls.getStateKey(state);
		return (key && engine.app.clearKeyState(key));
	}
	
	if (!engine.editor.isLoaded())
	{
		if (get_key_state(CONTROLS_STATE_SAVE))
			engine.console.run("state_save");
		if (get_key_state(CONTROLS_STATE_RESTORE))
			engine.console.run("state_restore");
		if (get_key_state(CONTROLS_STATE_SCREENSHOT))
			engine.console.run("video_grab");
	}

// gpu monitor
#ifdef HAS_GPU_MONITOR
	if (gpu_l == NULL)
	{
		engine.console.addCommand("show_gpu", "show gpu monitor", "System::show_gpu");
		gpu_l = new WidgetLabel(engine.getGui());
		gpu_l.setFontPermanent(1);
		gpu_l.setFontVSpacing(3);
		gpu_l.setFontOutline(1);
		gpu_l.setFontRich(1);
	}
	if (engine.config.getInt("show_gpu", SHOW_GPU))
	{
		string text = "";
		forloop(int i = 0; engine.gpumonitor.getNumMonitors())
		{
			GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
			forloop(int j = 0; monitor.getNumAdapters())
			{
				float core = monitor.getCoreClock(j);
				float memory = monitor.getMemoryClock(j);
				float shader = monitor.getShaderClock(j);
				float temperature = monitor.getTemperature(j);
				float utilization = monitor.getUtilization(j);
				
				if (utilization <= 15.0f && utilization != -1)
					continue;
				
				text += format("<p align=right><font size=14>%s</font></p>", monitor.getAdapterName(j));
				if (core > 0.0f)
					text += format("<p align=right><font size=11>Graphics: %d MHz</font></p>", core);
				if (memory > 0.0f)
					text += format("<p align=right><font size=11>Memory: %d MHz</font></p>", memory);
				if (shader > 0.0f)
					text += format("<p align=right><font size=11>Processor: %d MHz</font></p>", shader);
				if (temperature > -1000.0f)
					text += format("<p align=right><font size=11>Temperature: %d ºC</font></p>", temperature);
				if(utilization != -1)
					text += format("<p align=right><font size=11>Utilization: %3d%%</font></p>", utilization);
				text += format("<p align=right><font size=1> </font></p>");
			}
		}
		gpu_l.setText(text);
		gpu_l.arrange();
		int position_x = engine.app.getWidth() - gpu_l.getWidth() - 4;
		if (engine.console.getInt("show_fps"))
			gpu_l.setPosition(position_x, 28);
		else
			gpu_l.setPosition(position_x, 4);
		engine.gui.addChild(gpu_l, GUI_ALIGN_OVERLAP);
	} else
		engine.gui.removeChild(gpu_l);
#endif
}

void update_setting(WidgetCheckBox checkbox, string name, int value)
{
	if (checkbox == NULL)
		return;
	checkbox.setChecked(checkbox.isEnabled() && (engine.console.getInt(name) == value));
}

void update_setting(WidgetCheckBox checkbox, string name, int enable, int value)
{
	if (checkbox == NULL)
		return;
	if (is_int(enable))
		checkbox.setEnabled(enable);
	else if (is_string(enable))
		checkbox.setEnabled(engine.console.getInt(enable) == 0);
	checkbox.setChecked(checkbox.isEnabled() && (engine.console.getInt(name) == value));
}

void update_setting(WidgetCheckBox checkbox, string name, int enable, int values[], int value)
{
	if (checkbox == NULL)
		return;
	if (is_int(enable))
		checkbox.setEnabled(enable);
	else if (is_string(enable))
		checkbox.setEnabled(engine.console.getInt(enable) == 0);
	checkbox.setChecked(checkbox.isEnabled() && values.find(engine.console.getInt(name)) != -1);
}

int check_setting(WidgetCheckBox checkbox, string name, int value)
{
	if (checkbox == NULL || checkbox.isEnabled() == 0)
		return 0;
	return (engine.console.getInt(name) != checkbox.isChecked() * value);
}

void apply_setting(WidgetCheckBox checkbox, string name, int value)
{
	if (checkbox == NULL || checkbox.isEnabled() == 0)
		return;
	engine.console.setInt(name, checkbox.isChecked() * value);
}

//////////////////////////////////////////////////////////////////////////
// Video
//////////////////////////////////////////////////////////////////////////

namespace Video
{
string video_app;

WidgetCheckBox opengl_cb; // video app
WidgetCheckBox direct3d11_cb;

WidgetComboBox mode_cb;		  // video mode number
WidgetCheckBox fullscreen_cb; // fullscreen toggle
WidgetCheckBox vsync_cb;	  // vertical sync toggle

void init()
{
#ifdef HAS_OPENGL
	if (opengl_cb != NULL)
		opengl_cb.setEnabled(1);
#endif
#ifdef HAS_DIRECT3D11
	if (direct3d11_cb != NULL)
		direct3d11_cb.setEnabled(1);
#endif

	video_app = engine.getVideoApp();
}

void update()
{
	// video app
	if (opengl_cb != NULL && video_app == "opengl")
		opengl_cb.setChecked(1);
	if (direct3d11_cb != NULL && video_app == "direct3d11")
		direct3d11_cb.setChecked(1);

	// video mode
	if (mode_cb != NULL)
		mode_cb.setCurrentItem(engine.console.getInt("video_mode"));
	update_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN);
	update_setting(vsync_cb, "video_vsync", 1);
}

void update_dynamic()
{
}

int is_app_changed()
{
	if (opengl_cb != NULL && video_app == "opengl" && opengl_cb.isChecked() == 0)
		return 1;
	if (direct3d11_cb != NULL && video_app == "direct3d11" && direct3d11_cb.isChecked() == 0)
		return 1;
	return 0;
}

int is_mode_changed()
{
	if (mode_cb != NULL && mode_cb.getCurrentItem() != engine.console.getInt("video_mode"))
		return 1;
	if (check_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN))
		return 1;
	if (check_setting(vsync_cb, "video_vsync", 1))
		return 1;
	return 0;
}

int is_changed()
{
	if (is_app_changed())
		return 1;
	if (is_mode_changed())
		return 1;
	return 0;
}

void apply()
{
	// video app
	if (opengl_cb != NULL && opengl_cb.isChecked())
		video_app = "opengl";
	if (direct3d11_cb != NULL && direct3d11_cb.isChecked())
		video_app = "direct3d11";

	if (video_app != engine.getVideoApp())
		thread("System::need_restart");

	engine.config.setString("video_app", video_app);

	// video mode
	if (is_mode_changed())
	{
		update_splash();
		engine.console.setInt("video_mode", mode_cb.getCurrentItem());
		apply_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN);
		apply_setting(vsync_cb, "video_vsync", 1);
		engine.console.run("video_restart mode");
	}
}
}

//////////////////////////////////////////////////////////////////////////
// Render
//////////////////////////////////////////////////////////////////////////

namespace Render
{
WidgetComboBox textures_quality_cb; // textures quality
WidgetCheckBox textures_low_cb;
WidgetCheckBox textures_medium_cb;
WidgetCheckBox textures_high_cb;

WidgetComboBox filter_cb;	 // texture filter
WidgetComboBox anisotropy_cb; // texture anisotropy

WidgetCheckBox fxaa_cb;
WidgetCheckBox taa_cb;
void init()
{
}

void update()
{
	// textures
	int textures = engine.console.getInt("render_textures_quality");
	if (textures_quality_cb != NULL)
		textures_quality_cb.setCurrentItem(textures);
	if (textures_low_cb != NULL)
		textures_low_cb.setChecked((textures == RENDER_QUALITY_LOW));
	if (textures_medium_cb != NULL)
		textures_medium_cb.setChecked((textures == RENDER_QUALITY_MEDIUM));
	if (textures_high_cb != NULL)
		textures_high_cb.setChecked((textures == RENDER_QUALITY_HIGH));
	if (filter_cb != NULL)
		filter_cb.setCurrentItem(engine.console.getInt("render_textures_filter"));
	if (anisotropy_cb != NULL)
		anisotropy_cb.setCurrentItem(engine.console.getInt("render_textures_anisotropy"));

	// options
	if (fxaa_cb != NULL)
		fxaa_cb.setChecked(fxaa_cb.isEnabled() && (engine.console.getInt("render_fxaa") == 1));
	if (taa_cb != NULL)
		taa_cb.setChecked(taa_cb.isEnabled() && (engine.console.getInt("render_taa") == 1));
}

void update_dynamic()
{
}

int is_changed()
{
	// textures
	int textures = engine.console.getInt("render_textures_quality");
	if (textures_quality_cb != NULL && textures_quality_cb.getCurrentItem() != textures)
		return 1;
	if (textures_low_cb != NULL && textures_low_cb.isChecked() != (textures == RENDER_QUALITY_LOW))
		return 1;
	if (textures_medium_cb != NULL && textures_medium_cb.isChecked() != (textures == RENDER_QUALITY_MEDIUM))
		return 1;
	if (textures_high_cb != NULL && textures_high_cb.isChecked() != (textures == RENDER_QUALITY_HIGH))
		return 1;
	if (filter_cb != NULL && filter_cb.getCurrentItem() != engine.console.getInt("render_textures_filter"))
		return 1;
	if (anisotropy_cb != NULL && anisotropy_cb.getCurrentItem() != engine.console.getInt("render_textures_anisotropy"))
		return 1;

	// options
	if (fxaa_cb != NULL && fxaa_cb.isChecked() != engine.console.getInt("render_fxaa"))
		return 1;
	if (taa_cb != NULL && taa_cb.isChecked() != engine.console.getInt("render_taa"))
		return 1;

	return 0;
}

void apply()
{
	// textures
	if (textures_quality_cb != NULL)
		engine.console.setInt("render_textures_quality", textures_quality_cb.getCurrentItem());
	if (textures_low_cb != NULL && textures_low_cb.isChecked())
		engine.console.setInt("render_textures_quality", RENDER_QUALITY_LOW);
	if (textures_medium_cb != NULL && textures_medium_cb.isChecked())
		engine.console.setInt("render_textures_quality", RENDER_QUALITY_MEDIUM);
	if (textures_high_cb != NULL && textures_high_cb.isChecked())
		engine.console.setInt("render_textures_quality", RENDER_QUALITY_HIGH);
	if (filter_cb != NULL)
		engine.console.setInt("render_textures_filter", filter_cb.getCurrentItem());
	if (anisotropy_cb != NULL)
		engine.console.setInt("render_textures_anisotropy", anisotropy_cb.getCurrentItem());

	// options
	if (fxaa_cb != NULL)
		engine.console.setInt("render_fxaa", fxaa_cb.isChecked());
	if (taa_cb != NULL)
		engine.console.setInt("render_taa", taa_cb.isChecked());

}

} // namespace Render

//////////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////////

namespace Interface
{
WidgetCheckBox fps_cb;		// fps counter toggle
WidgetCheckBox gpu_cb;		// gpu monitor toggle
WidgetCheckBox messages_cb; // messages toggle
WidgetCheckBox profiler_cb; // profiler toggle

WidgetCheckBox triangles_cb; // show triangles toggle

WidgetComboBox language_cb; // system language

void init()
{
	::System::language_cb = language_cb;
}

void update()
{
	// engine
	update_setting(fps_cb, "show_fps", 1);
	update_setting(messages_cb, "show_messages", 1);
	update_setting(profiler_cb, "show_profiler", 1);

	// render show
	update_setting(triangles_cb, "render_show_triangles", 1);

// gpu monitor
#ifdef HAS_GPU_MONITOR
	if (gpu_cb != NULL)
		gpu_cb.setChecked(engine.config.getInt("show_gpu", SHOW_GPU));
#else
	if (gpu_cb != NULL)
		gpu_cb.setHidden(1);
#endif
}

void update_dynamic()
{
}

int is_changed()
{
	// engine
	if (check_setting(fps_cb, "show_fps", 1))
		return 1;
	if (check_setting(messages_cb, "show_messages", 1))
		return 1;
	if (check_setting(profiler_cb, "show_profiler", 1))
		return 1;

	// render show
	if (check_setting(triangles_cb, "render_show_triangles", 1))
		return 1;

	// language
	if (language_cb != NULL && language_cb.getCurrentItem() != language)
		return 1;

// gpu monitor
#ifdef HAS_GPU_MONITOR
	if (gpu_cb != NULL && gpu_cb.isChecked() != engine.config.getInt("show_gpu", SHOW_GPU))
		return 1;
#endif

	return 0;
}

void apply()
{
	// engine
	apply_setting(fps_cb, "show_fps", 1);
	apply_setting(messages_cb, "show_messages", 1);
	apply_setting(profiler_cb, "show_profiler", 1);

	// render show
	apply_setting(triangles_cb, "render_show_triangles", 1);

	// language
	if (language_cb != NULL)
		set_language(language_cb.getCurrentItem());

// gpu monitor
#ifdef HAS_GPU_MONITOR
	if (gpu_cb != NULL)
		engine.config.setInt("show_gpu", gpu_cb.isChecked());
#endif
}
}

//////////////////////////////////////////////////////////////////////////
// Settings
//////////////////////////////////////////////////////////////////////////

namespace Settings
{
WidgetComboBox mode_cb;		  // video mode number
WidgetCheckBox fullscreen_cb; // fullscreen toggle
WidgetCheckBox vsync_cb;	  // vertical sync toggle

WidgetCheckBox fps_cb;		// fps counter toggle
WidgetCheckBox gpu_cb;		// gpu monitor toggle
WidgetCheckBox messages_cb; // messages toggle
WidgetCheckBox profiler_cb; // profiler toggle

WidgetCheckBox triangles_cb; // show triangles toggle

WidgetComboBox language_cb; // system language

void init()
{
	::System::language_cb = language_cb;
}

void update()
{
	// video mode
	if (mode_cb != NULL)
		mode_cb.setCurrentItem(engine.console.getInt("video_mode"));
	update_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN);
	update_setting(vsync_cb, "video_vsync", 1);

	// interface settings
	update_setting(fps_cb, "show_fps", 1);
	update_setting(messages_cb, "show_messages", 1);
	update_setting(profiler_cb, "show_profiler", 1);

	// render show
	update_setting(triangles_cb, "render_show_triangles", 1);

// gpu monitor
#ifdef HAS_GPU_MONITOR
	if (gpu_cb != NULL)
		gpu_cb.setChecked(engine.config.getInt("show_gpu", SHOW_GPU));
#else
	if (gpu_cb != NULL)
		gpu_cb.setHidden(1);
#endif
}

void update_dynamic()
{
}

int is_mode_changed()
{
	if (mode_cb != NULL && mode_cb.getCurrentItem() != engine.console.getInt("video_mode"))
		return 1;
	if (check_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN))
		return 1;
	if (check_setting(vsync_cb, "video_vsync", 1))
		return 1;
	return 0;
}

int is_interface_changed()
{
	if (check_setting(fps_cb, "show_fps", 1))
		return 1;
	if (check_setting(messages_cb, "show_messages", 1))
		return 1;
	if (check_setting(profiler_cb, "show_profiler", 1))
		return 1;
	if (check_setting(triangles_cb, "render_show_triangles", 1))
		return 1;
	if (language_cb != NULL && language_cb.getCurrentItem() != language)
		return 1;
#ifdef HAS_GPU_MONITOR
	if (gpu_cb != NULL && gpu_cb.isChecked() != engine.config.getInt("show_gpu", SHOW_GPU))
		return 1;
#endif
	return 0;
}

int is_changed()
{
	if (is_mode_changed())
		return 1;
	if (is_interface_changed())
		return 1;
	return 0;
}

void apply()
{
	// video mode
	if (is_mode_changed())
	{
		update_splash();
		engine.console.setInt("video_mode", mode_cb.getCurrentItem());
		apply_setting(fullscreen_cb, "video_fullscreen", VIDEO_FULLSCREEN);
		apply_setting(vsync_cb, "video_vsync", 1);
		engine.console.run("video_restart mode");
	}

	// interface settings
	if (is_interface_changed())
	{
		apply_setting(fps_cb, "show_fps", 1);
		apply_setting(messages_cb, "show_messages", 1);
		apply_setting(profiler_cb, "show_profiler", 1);
		apply_setting(triangles_cb, "render_show_triangles", 1);
		if (language_cb != NULL)
			set_language(language_cb.getCurrentItem());
#ifdef HAS_GPU_MONITOR
		if (gpu_cb != NULL)
			engine.config.setInt("show_gpu", gpu_cb.isChecked());
#endif
	}
}
}

//////////////////////////////////////////////////////////////////////////
// Controls
//////////////////////////////////////////////////////////////////////////

namespace Controls
{
WidgetCheckBox always_run_cb;
WidgetCheckBox mouse_inverse_cb;
WidgetSlider mouse_sensitivity_sl;

WidgetButton forward_b;
WidgetButton backward_b;
WidgetButton move_left_b;
WidgetButton move_right_b;
WidgetButton turn_up_b;
WidgetButton turn_down_b;
WidgetButton turn_left_b;
WidgetButton turn_right_b;
WidgetButton crouch_b;
WidgetButton jump_b;
WidgetButton run_b;
WidgetButton use_b;
WidgetButton fire_b;
WidgetButton save_b;
WidgetButton restore_b;
WidgetButton screenshot_b;
WidgetButton aux_0_b;
WidgetButton aux_1_b;
WidgetButton aux_2_b;
WidgetButton aux_3_b;
WidgetButton aux_4_b;
WidgetButton aux_5_b;
WidgetButton aux_6_b;
WidgetButton aux_7_b;
WidgetButton aux_8_b;
WidgetButton aux_9_b;
WidgetButton aux_a_b;
WidgetButton aux_b_b;
WidgetButton aux_c_b;
WidgetButton aux_d_b;
WidgetButton aux_e_b;
WidgetButton aux_f_b;

void init()
{
	// check config
	int init = 1;
	for (int i = 0; i < CONTROLS_STATE_AUX_0; i++)
	{
		if (engine.config.isExist(format("controls_key_%d", i)) || engine.config.isExist(format("controls_button_%d", i)))
		{
			init = 0;
			break;
		}
	}

	// setup default controls
	if (init)
	{
		engine.controls.setStateKey(CONTROLS_STATE_FORWARD, 'w');
		engine.controls.setStateKey(CONTROLS_STATE_BACKWARD, 's');
		engine.controls.setStateKey(CONTROLS_STATE_MOVE_LEFT, 'a');
		engine.controls.setStateKey(CONTROLS_STATE_MOVE_RIGHT, 'd');
		engine.controls.setStateKey(CONTROLS_STATE_TURN_UP, APP_KEY_UP);
		engine.controls.setStateKey(CONTROLS_STATE_TURN_DOWN, APP_KEY_DOWN);
		engine.controls.setStateKey(CONTROLS_STATE_TURN_LEFT, APP_KEY_LEFT);
		engine.controls.setStateKey(CONTROLS_STATE_TURN_RIGHT, APP_KEY_RIGHT);
		engine.controls.setStateKey(CONTROLS_STATE_CROUCH, 'q');
		engine.controls.setStateKey(CONTROLS_STATE_JUMP, 'e');
		engine.controls.setStateKey(CONTROLS_STATE_RUN, APP_KEY_SHIFT);
		engine.controls.setStateKey(CONTROLS_STATE_USE, APP_KEY_RETURN);
		engine.controls.setStateKey(CONTROLS_STATE_SAVE, APP_KEY_F5);
		engine.controls.setStateKey(CONTROLS_STATE_RESTORE, APP_KEY_F6);
		engine.controls.setStateKey(CONTROLS_STATE_SCREENSHOT, APP_KEY_F12);
		engine.controls.setStateButton(CONTROLS_STATE_FIRE, APP_BUTTON_LEFT);
	}
}

void update()
{
	if (always_run_cb != NULL)
		always_run_cb.setChecked(engine.controls.getAlwaysRun());
	if (mouse_inverse_cb != NULL)
		mouse_inverse_cb.setChecked(engine.controls.getMouseInverse());
	if (mouse_sensitivity_sl != NULL)
		mouse_sensitivity_sl.setValue(engine.controls.getMouseSensitivity() * 1000.0f);

	// states
	if (forward_b != NULL)
		forward_b.setText(engine.controls.getStateName(CONTROLS_STATE_FORWARD));
	if (backward_b != NULL)
		backward_b.setText(engine.controls.getStateName(CONTROLS_STATE_BACKWARD));
	if (move_left_b != NULL)
		move_left_b.setText(engine.controls.getStateName(CONTROLS_STATE_MOVE_LEFT));
	if (move_right_b != NULL)
		move_right_b.setText(engine.controls.getStateName(CONTROLS_STATE_MOVE_RIGHT));
	if (turn_up_b != NULL)
		turn_up_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_UP));
	if (turn_down_b != NULL)
		turn_down_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_DOWN));
	if (turn_left_b != NULL)
		turn_left_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_LEFT));
	if (turn_right_b != NULL)
		turn_right_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_RIGHT));
	if (crouch_b != NULL)
		crouch_b.setText(engine.controls.getStateName(CONTROLS_STATE_CROUCH));
	if (jump_b != NULL)
		jump_b.setText(engine.controls.getStateName(CONTROLS_STATE_JUMP));
	if (run_b != NULL)
		run_b.setText(engine.controls.getStateName(CONTROLS_STATE_RUN));
	if (use_b != NULL)
		use_b.setText(engine.controls.getStateName(CONTROLS_STATE_USE));
	if (fire_b != NULL)
		fire_b.setText(engine.controls.getStateName(CONTROLS_STATE_FIRE));
	if (save_b != NULL)
		save_b.setText(engine.controls.getStateName(CONTROLS_STATE_SAVE));
	if (restore_b != NULL)
		restore_b.setText(engine.controls.getStateName(CONTROLS_STATE_RESTORE));
	if (screenshot_b != NULL)
		screenshot_b.setText(engine.controls.getStateName(CONTROLS_STATE_SCREENSHOT));
	if (aux_0_b != NULL)
		aux_0_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_0));
	if (aux_1_b != NULL)
		aux_1_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_1));
	if (aux_2_b != NULL)
		aux_2_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_2));
	if (aux_3_b != NULL)
		aux_3_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_3));
	if (aux_4_b != NULL)
		aux_4_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_4));
	if (aux_5_b != NULL)
		aux_5_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_5));
	if (aux_6_b != NULL)
		aux_6_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_6));
	if (aux_7_b != NULL)
		aux_7_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_7));
	if (aux_8_b != NULL)
		aux_8_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_8));
	if (aux_9_b != NULL)
		aux_9_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_9));
	if (aux_a_b != NULL)
		aux_a_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_A));
	if (aux_b_b != NULL)
		aux_b_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_B));
	if (aux_c_b != NULL)
		aux_c_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_C));
	if (aux_d_b != NULL)
		aux_d_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_D));
	if (aux_e_b != NULL)
		aux_e_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_E));
	if (aux_f_b != NULL)
		aux_f_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_F));
}

void update_dynamic()
{
	if (engine.controls.isStateEvent())
	{
#ifdef HAS_INTERFACE
		event_i.setWidget(NULL);
		event_i.setHidden(1);
#else
		engine.gui.removeChild(event_w);
#endif
		update();
	}
}

int is_changed()
{
	if (always_run_cb != NULL && always_run_cb.isChecked() != engine.controls.getAlwaysRun())
		return 1;
	if (mouse_inverse_cb != NULL && mouse_inverse_cb.isChecked() != engine.controls.getMouseInverse())
		return 1;
	if (mouse_sensitivity_sl != NULL && mouse_sensitivity_sl.getValue() / 1000.0f != engine.controls.getMouseSensitivity())
		return 1;
	return 0;
}

void apply()
{
	if (always_run_cb != NULL)
		engine.controls.setAlwaysRun(always_run_cb.isChecked());
	if (mouse_inverse_cb != NULL)
		engine.controls.setMouseInverse(mouse_inverse_cb.isChecked());
	if (mouse_sensitivity_sl != NULL)
		engine.controls.setMouseSensitivity(mouse_sensitivity_sl.getValue() / 1000.0f);
}

void event_clicked(string state)
{
	if (event_w == NULL)
	{
		log.error("System::Controls::event_clicked(): event window is NULL\n");
		return;
	}

	int states[] = ("forward"
					: CONTROLS_STATE_FORWARD,
					"backward"
					: CONTROLS_STATE_BACKWARD,
					"move_left"
					: CONTROLS_STATE_MOVE_LEFT,
					"move_right"
					: CONTROLS_STATE_MOVE_RIGHT,
					"turn_up"
					: CONTROLS_STATE_TURN_UP,
					"turn_down"
					: CONTROLS_STATE_TURN_DOWN,
					"turn_left"
					: CONTROLS_STATE_TURN_LEFT,
					"turn_right"
					: CONTROLS_STATE_TURN_RIGHT,
					"crouch"
					: CONTROLS_STATE_CROUCH,
					"jump"
					: CONTROLS_STATE_JUMP,
					"run"
					: CONTROLS_STATE_RUN,
					"use"
					: CONTROLS_STATE_USE,
					"fire"
					: CONTROLS_STATE_FIRE,
					"save"
					: CONTROLS_STATE_SAVE,
					"restore"
					: CONTROLS_STATE_RESTORE,
					"screenshot"
					: CONTROLS_STATE_SCREENSHOT,

					"aux_0"
					: CONTROLS_STATE_AUX_0, "aux_1"
					: CONTROLS_STATE_AUX_1,
					"aux_2"
					: CONTROLS_STATE_AUX_2, "aux_3"
					: CONTROLS_STATE_AUX_3,
					"aux_4"
					: CONTROLS_STATE_AUX_4, "aux_5"
					: CONTROLS_STATE_AUX_5,
					"aux_6"
					: CONTROLS_STATE_AUX_6, "aux_7"
					: CONTROLS_STATE_AUX_7,
					"aux_8"
					: CONTROLS_STATE_AUX_8, "aux_9"
					: CONTROLS_STATE_AUX_9,
					"aux_a"
					: CONTROLS_STATE_AUX_A, "aux_b"
					: CONTROLS_STATE_AUX_B,
					"aux_c"
					: CONTROLS_STATE_AUX_C, "aux_d"
					: CONTROLS_STATE_AUX_D,
					"aux_e"
					: CONTROLS_STATE_AUX_E, "aux_f"
					: CONTROLS_STATE_AUX_F, );

	if (states.check(state) == 0)
	{
		log.error("System::Controls::event_clicked(): unknown event state \"%s\"\n", state);
		return;
	}

	engine.controls.getStateEvent(states[state]);

#ifdef HAS_INTERFACE
	if (event_i == NULL)
		event_i = new InterfaceWindow(0);
	event_i.setWidget(event_w);
	event_i.setCenterPosition();
	event_i.setHidden(0);
	event_i.setPermanentFocus();
	event_w.setPermanentFocus();
#else
	engine.gui.addChild(event_w, GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
	event_w.setPermanentFocus();
#endif
}

} // namespace Controls

//////////////////////////////////////////////////////////////////////////
// Plugins
//////////////////////////////////////////////////////////////////////////

namespace Plugins
{
	WidgetGroupBox syncker_gb;
	WidgetCheckBox debug_cb;
	WidgetCheckBox debug_window_cb;
	WidgetButton projections_cb;
	
	void debug_clicked() 
	{
		#ifdef HAS_SYNCKER
		if (engine.syncker.isSynckerInitialized())
		{
			SynckerInterface syncker = engine.syncker.getSyncker();
			syncker.setDebug(debug_cb.isChecked(), 10, 10, 20);
		}
		#endif
	}
	
	void debug_window_clicked()
	{
		#ifdef HAS_SYNCKER
		engine.syncker.setDebugWindow(debug_window_cb.isChecked());
		#endif
	}
	
	void projections_clicked()
	{
		#ifdef HAS_SYNCKER
		if (engine.syncker.isSynckerInitialized())
		{
			SynckerInterface syncker = engine.syncker.getSyncker();
			syncker.setSetupMode(1);
		}
		#endif
	}
	
	void init()
	{
		#ifdef HAS_SYNCKER
		syncker_gb.setHidden(0);
		if (!engine.syncker.isSynckerInitialized())
		{
			debug_cb.setEnabled(0);
			projections_cb.setEnabled(0);
		}
		#else
		syncker_gb.setHidden(1);
		debug_cb.setEnabled(0);
		debug_window_cb.setEnabled(0);
		projections_cb.setEnabled(0);
		#endif
	}
	
	void update()
	{
		#ifdef HAS_SYNCKER
		int active = engine.syncker.isSynckerInitialized();
		
		if (active)
		{
			SynckerInterface syncker = engine.syncker.getSyncker();
			debug_cb.setChecked(syncker.isDebug());
		}

		debug_window_cb.setChecked(engine.syncker.isDebugWindow());
		
		debug_cb.setEnabled(active);
		projections_cb.setEnabled(active);
		#endif
	}
	
	void update_dynamic()
	{
	}

	int is_changed()
	{
	}

	void apply()
	{
	}

	void event_clicked(string state)
	{
	}
}
} // namespace System

//////////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////////

void systemInit(string name, string languages[], string locales[])
{
	System::init(name, languages, locales);
}

void systemInit(string name = "core/unigine.ui")
{
	string languages[] = ("English", "Russian");
	string locales[] = ("core/locale/unigine.locale:en", "core/locale/unigine.locale:ru");
	System::init(name, languages, locales);
}

void systemShutdown()
{
	System::shutdown();
}

void systemUpdate()
{
	System::update();
}

void systemQuit()
{
	thread("System::quit_clicked");
}

void systemShow()
{
	System::show();
}

void systemHide()
{
	System::hide();
}

void systemSetMouseUser(int enabled)
{
	System::set_mouse_user(enabled);
}

int systemIsMouseUser()
{
	return System::is_mouse_user();
}

void systemSetToggle(int toggle)
{
	System::set_toggle(toggle);
}

int systemGetToggle()
{
	return System::get_toggle();
}

int systemGetActivity()
{
	return System::get_activity();
}

void systemSetLanguage(int language)
{
	System::set_language(language);
}

int systemGetLanguage()
{
	return System::get_language();
}

void systemSetLanguageCallback(string callback)
{
	System::language_callback = callback;
}

string systemGetLanguageCallback()
{
	return System::language_callback;
}

void systemSetLanguageWorldCallback(string callback)
{
	System::language_world_callback = callback;
}

string systemGetLanguageWorldCallback()
{
	return System::language_world_callback;
}

void systemSetLanguageEditorCallback(string callback)
{
	System::language_editor_callback = callback;
}

string systemGetLanguageEditorCallback()
{
	return System::language_editor_callback;
}

void systemSetSplash(string splash_4x3_name, string splash_16x9_name, int splash_threshold = 16,
	vec4 splash_transform = vec4(1.0f, 1.0f, 0.0f, 0.0f), vec4 splash_color = vec4(0.0f, 0.0f, 0.0f, 1.0f), string splash_text = "")
{
	System::splash_4x3_name = splash_4x3_name;
	System::splash_16x9_name = splash_16x9_name;
	System::splash_threshold = splash_threshold;
	System::splash_transform = splash_transform;
	System::splash_color = splash_color;
	System::splash_text = splash_text;

	System::update_splash();
}

void systemSetSplash(string splash_4x3_name, string splash_16x9_name, int splash_threshold = 16)
{
	System::splash_4x3_name = splash_4x3_name;
	System::splash_16x9_name = splash_16x9_name;
	System::splash_threshold = splash_threshold;
	System::splash_transform = vec4(1.0f, 1.0f, 0.0f, 0.0f);
	System::splash_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	System::splash_text = "";

	System::update_splash();
}

WidgetWindow systemGetWindow()
{
	return System::main_w;
}

void systemSetWindowText(string text)
{
	return System::main_w.setText(text);
}

#endif /* __UNIGINE_SYSTEM_H__ */
