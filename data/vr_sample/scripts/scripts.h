#ifndef __SCRIPTS_H_INCLUDED__
#define __SCRIPTS_H_INCLUDED__

#include <core/unigine.h>

namespace Scripts
{
	class NamePool
	{
	private:
		string base_name;
		int counter = 0;
		string used_names[0] = ();
		string free_names[0] = ();

	public:
		NamePool(string name)
		{
			base_name = name;
			clear();
		}

		void clear()
		{
			counter = 0;
			used_names.clear();
			free_names.clear();
		}

		int getNumNames() { return used_names.size(); }
		string getName(int index) { return used_names[index]; }

		string fetchUniqueName()
		{
			string name = base_name;

			// Use from free pool
			if(free_names.size() > 0)
			{
				int last = free_names.size() - 1;
				name = free_names[last];
				free_names.remove(last);
			}
			// Allocate new name
			else
			{
				counter++;
				name = format("%s_%d", base_name, counter);
			}

			used_names.append(name);
			return name;
		}

		void releaseName(string name)
		{
			int index = used_names.find(name);
			if (index == -1)
				return;

			free_names.append(used_names[index]);
			used_names.remove(index);
		}
	};

	class ClassBinding
	{
	private:
		NamePool name_pool;
		string display_name;
		string node_path;

	public:
		ClassBinding(string name, string path)
		{
			node_path = path;
			display_name = name;
			name_pool = new NamePool(name);
		}

		NamePool getNamePool() { return name_pool; }
		string getDisplayName() { return display_name; }

		Node createInstance()
		{
			Node node = node_remove(node_load(node_path));
			engine.editor.addNode(node, 1);
			node.setName(name_pool.fetchUniqueName());

			return node;
		}
	};

	ObjScript logic;
	int logic_initialized = 0;
	int gui_initialized = 0;

	ClassBinding bindings[0] = ();

	int registerClass(string display_name, string node_path)
	{
		ClassBinding binding = new ClassBinding(display_name, node_path);
		bindings.append(binding);
	}

	int init()
	{
		engine.console.addCommand("reload_logic", "Reloads script main logic", "Scripts::reload_logic");
		return 1;
	}

	int init_logic()
	{
		if (logic_initialized)
			return;

		logic = scripts.getScriptByNodeName("logic");
		logic_initialized = 1;
	}

	int init_gui()
	{
		if (gui_initialized)
			return;

		Gui gui = scripts.getGui();
		if (gui == NULL)
			return;

		// Background
		WidgetSprite background = new WidgetSprite(gui, "black.dds");
		background.setColor(vec4(1, 1, 1, 0.25f));

		background.setWidth(gui.getWidth());
		background.setHeight(gui.getHeight());

		// Buttons
		WidgetGroupBox buttons_gb = new WidgetGroupBox(gui, "Create Object");
		buttons_gb.setFontSize(32);

		forloop(int i = 0; bindings.size())
		{
			WidgetButton button = new WidgetButton(gui, bindings[i].getDisplayName());
			button.setToggleable(0);
			button.setFontSize(32);
			button.setCallback(GUI_CLICKED, functionid(create_button_clicked), bindings[i]);

			buttons_gb.addChild(button, GUI_ALIGN_EXPAND);
		}

		// Controls
		WidgetGroupBox controls_gb = new WidgetGroupBox(gui, "Controls");
		controls_gb.setFontSize(32);

		WidgetButton reload_button = new WidgetButton(gui, "Reload Logic");
		reload_button.setToggleable(0);
		reload_button.setFontSize(32);
		reload_button.setCallback(GUI_CLICKED, functionid(reload_logic));
		controls_gb.addChild(reload_button, GUI_ALIGN_EXPAND);

		WidgetCheckBox checkbox = new WidgetCheckBox(gui, "Show Names");
		checkbox.setCallback(GUI_CHANGED, functionid(show_names_toggled));
		checkbox.setFontSize(32);
		controls_gb.addChild(checkbox, GUI_ALIGN_EXPAND);

		gui.addChild(buttons_gb, GUI_ALIGN_EXPAND);
		gui.addChild(controls_gb, GUI_ALIGN_EXPAND);
		gui.addChild(background, GUI_ALIGN_BACKGROUND | GUI_ALIGN_OVERLAP);

		gui_initialized = 1;
	}

	int update()
	{
		init_gui();
		init_logic();
		return 1;
	}

	int render()
	{
		int font_size = 42;
		forloop(int i = 0; bindings.size())
		{
			NamePool pool = bindings[i].getNamePool();
			forloop(int j = 0; pool.getNumNames())
			{
				string name = pool.getName(j);
				ObjScript script = scripts.getScriptByNodeName(name);
				if (script == NULL)
					continue;

				Node node = script.getNode();
				engine.visualizer.renderMessage3D(node.getWorldPosition(), vec3_zero, name, vec4_one, 1, font_size);
			}
		}

		return 1;
	}

	int flush()
	{
		return 1;
	}

	int shutdown()
	{
		engine.console.removeCommand("reload_logic");
		return 1;
	}

	void create_button_clicked(ClassBinding binding)
	{
		Node node = binding.createInstance();

		if (scripts.getNumHands() > 0)
		{
			Node left_hand = scripts.getHandNode(0);
			Mat4 left_hand_transform = left_hand.getWorldTransform();
			node.setWorldPosition(left_hand.getWorldPosition() - left_hand_transform.col23 * 0.5f);
		}
	}

	void show_names_toggled()
	{
		engine.visualizer.setEnabled(!engine.visualizer.isEnabled());
	}

	void reload_logic()
	{
		if (logic == NULL)
			return;

		logic.reload();
	}
}

#endif /* __SCRIPTS_H_INCLUDED__ */
