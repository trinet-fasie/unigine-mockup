/* Animation test
ScriptAnimation animation;
ScriptButton button;

void play_random_animation()
{
	if (animation == NULL)
		return;

	if (animation.getNumAnimations() == 0)
		return;

	animation.play(rand(0, animation.getNumAnimations()));
}

void init(ObjScript script)
{
	animation = scripts.getScriptClassByNodeName("Animation_1");
	button = scripts.getScriptClassByNodeName("Button_1");

	if (button != NULL)
		button.setPressedCallback(functionid(play_random_animation));

	if (animation != NULL)
	{
		animation.stop();
		animation.setLooped(1);
		animation.setSpeed(30.0f);
	}
}
/**/

/* Display test
ScriptDisplay display;

void init(ObjScript script)
{
	display = scripts.getScriptClassByNodeName("Display_1");
	if (display != NULL)
	{
		display.setFontSize(60);
		display.setText("Hello, World!");
	}
}
/**/

/* Button / Switcher / Lamp test
ScriptButton button;
ScriptSwitcher switcher;
ScriptLamp lamp1;
ScriptLamp lamp2;

void update_colors()
{
	update_lamp_color(lamp1);
	update_lamp_color(lamp2);
}

void update_lamp_color(ScriptLamp lamp)
{
	if (lamp == NULL)
		return;

	lamp.setColor(vec4(rand(), rand(), rand(), 1.0f));
}

void update_lights()
{
	if (switcher == NULL)
		return;

	if (lamp1 == NULL || lamp2 == NULL)
		return;

	int toggled = switcher.isToggled();
	lamp1.setEnabled(toggled);
	lamp2.setEnabled(toggled);
}

void init(ObjScript script)
{
	lamp1 = scripts.getScriptClassByNodeName("Lamp_1");
	if (lamp1 != NULL)
		lamp1.setEnabled(1);

	lamp2 = scripts.getScriptClassByNodeName("Lamp_2");
	if (lamp2 != NULL)
		lamp2.setEnabled(1);

	button = scripts.getScriptClassByNodeName("Button_1");

	if (button != NULL)
		button.setPressedCallback(functionid(update_colors));

	switcher = scripts.getScriptClassByNodeName("Switcher_1");
	if (switcher != NULL)
		switcher.setChangedCallback(functionid(update_lights));
}
/**/
