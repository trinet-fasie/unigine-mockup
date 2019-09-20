#ifndef __SCRIPT_BUTTON_H_INCLUDED__
#define __SCRIPT_BUTTON_H_INCLUDED__

class ScriptButton
{
private:
	ObjScript script;
	Node node;

	float animation_duration = 0.5f;
	Vec3 offset = Vec3(0.0f, 0.0f, -0.01f);

	Vec3 disabled_position;
	Vec3 enabled_position;
	Vec3 from_pos;
	int pressed_callback;
	float time;

public:
	ScriptButton(ObjScript s)
	{
		script = s;
		node = script.getNode();

		disabled_position = node.getPosition();
		enabled_position = disabled_position + offset;

		time = animation_duration;
	}

	void setPressedCallback(int c) { pressed_callback = c; }
	int getPressedCallback() { return pressed_callback; }

	void update()
	{
		if (time >= animation_duration)
			return;

		float ifps = engine.game.getIFps();
		time += ifps;

		float k = saturate(time / animation_duration);
		k = 1.0f - abs(k - 0.5f) * 2.0f; // triangle shape (0 -> 1 -> 0)
		node.setPosition(lerp(time < 0.5f ? from_pos : disabled_position, enabled_position, k));
	}

	void press()
	{
		time = 0.0f;

		from_pos = node.getPosition();

		if (pressed_callback != 0)
			call(pressed_callback);
	}
};

#endif /* __SCRIPT_BUTTON_H_INCLUDED__ */
