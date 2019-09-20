#ifndef __SCRIPT_DISPLAY_H_INCLUDED__
#define __SCRIPT_DISPLAY_H_INCLUDED__

class ScriptDisplay
{
private:
	ObjScript script;
	ObjectGuiMesh gui_mesh;
	WidgetLabel text;

public:
	ScriptDisplay(ObjScript s)
	{
		script = s;

		Node node = script.getNode();

		Node hierarchy[0] = ();
		node.getHierarchy(hierarchy);

		forloop(int i = 0; hierarchy.size())
		{
			if (hierarchy[i].getType() == NODE_OBJECT_GUI_MESH)
			{
				gui_mesh = node_cast(hierarchy[i]);
				break;
			}
		}

		Gui gui = gui_mesh.getGui();

		WidgetSprite background = new WidgetSprite(gui, "black.dds");

		background.setWidth(gui.getWidth());
		background.setHeight(gui.getHeight());

		text = new WidgetLabel(gui, "");
		text.setTextAlign(GUI_ALIGN_CENTER);
		text.setFontSize(32);

		gui.addChild(background, GUI_ALIGN_BACKGROUND | GUI_ALIGN_OVERLAP);
		gui.addChild(text, GUI_ALIGN_EXPAND | GUI_ALIGN_CENTER);
	}

	void setText(string value) { text.setText(value); }
	string getText() { return text.getText(); }

	void setFontRich(int value) { text.setFontRich(value); }
	int isFontRich() { return text.getFontRich(); }

	void setFontSize(int value) { text.setFontSize(value); }
	int getFontSize() { return text.getFontSize(); }

	void setEnabled(int enabled) { gui_mesh.setEnabled(enabled);  }
	int isEnabled() { return gui_mesh.isEnabled() }
};

#endif /* __SCRIPT_DISPLAY_H_INCLUDED__ */
