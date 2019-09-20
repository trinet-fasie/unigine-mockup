#ifndef __SCRIPT_SWITCHER_H_INCLUDED__
#define __SCRIPT_SWITCHER_H_INCLUDED__

class ScriptSwitcher
{
private:
	ObjScript script;
	Node node;

	float animation_duration = 0.1f;

	quat disabled_rotation = quat(0.0f, 0.0f, 0.0f);
	quat enabled_rotation = quat(0.0f, -15.0f, 0.0f);

	Vec3 from_rot;
	Vec3 to_rot;
	int changed_callback;
	int toggled;
	float time;

public:
	ScriptSwitcher(ObjScript s)
	{
		script = s;
		node = script.getNode();

		toggled = 0;
		time = animation_duration;
	}

	int isToggled() { return toggled; }

	void setChangedCallback(int c) { changed_callback = c; }
	int getChangedCallback() { return changed_callback; }

	void update()
	{
		if (time >= animation_duration)
			return;

		float ifps = engine.game.getIFps();
		time += ifps;

		float k = saturate(time / animation_duration);
		node.setRotation(lerp(from_rot, to_rot, k));
	}

	void toggle()
	{
		time = 0.0f;
		toggled = !toggled;

		from_rot = node.getRotation();
		to_rot = (toggled) ? enabled_rotation : disabled_rotation;

		if (changed_callback != 0)
			call(changed_callback);
	}
};

#endif /* __SCRIPT_SWITCHER_H_INCLUDED__ */
