#include <core/unigine.h>
#include <vr_sample/scripts/scripts.h>
#include <vr_sample/scripts/classes/script_animation.h>
#include <vr_sample/scripts/classes/script_button.h>
#include <vr_sample/scripts/classes/script_display.h>
#include <vr_sample/scripts/classes/script_lamp.h>
#include <vr_sample/scripts/classes/script_switcher.h>

int init()
{
	Player player = new PlayerSpectator();
	player.setPosition(Vec3(0.0f,-3.401f,1.5f));
	player.setDirection(Vec3(0.0f,1.0f,-0.4f));
	engine.game.setPlayer(player);

	Scripts::registerClass("Animation", "vr_sample/props/animation/animation.node");
	Scripts::registerClass("Button", "vr_sample/props/button/button.node");
	Scripts::registerClass("Display", "vr_sample/props/display/display.node");
	Scripts::registerClass("Lamp", "vr_sample/props/lamp/lamp.node");
	Scripts::registerClass("Switcher", "vr_sample/props/switcher/switcher.node");

	Scripts::init();
	return 1;
}

int update()
{
	Scripts::update();
	return 1;
}

int render()
{
	Scripts::render();
	return 1;
}

int flush()
{
	Scripts::flush();
	return 1;
}

int shutdown()
{
	Scripts::shutdown();
	return 1;
}
