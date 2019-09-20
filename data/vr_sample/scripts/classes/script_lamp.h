#ifndef __SCRIPT_LAMP_H_INCLUDED__
#define __SCRIPT_LAMP_H_INCLUDED__

class ScriptLamp
{
private:
	ObjScript script;
	Light light;

public:
	ScriptLamp(ObjScript s)
	{
		script = s;

		Node node = script.getNode();

		Node hierarchy[0] = ();
		node.getHierarchy(hierarchy);

		forloop(int i = 0; hierarchy.size())
		{
			if (hierarchy[i].isLight())
			{
				light = node_cast(hierarchy[i]);
				break;
			}
		}
		light.setColorMode(LIGHT_COLOR_MODE_CLASSIC);
	}

	void setColor(vec4 value) { light.setColor(value); }
	vec4 getColor() { return light.calculateFinalColor(); }

	void setEnabled(int enabled) { light.setEnabled(enabled);  }
	int isEnabled() { return light.isEnabled() }

};

#endif /* __SCRIPT_LAMP_H_INCLUDED__ */
