#include "VRPlayerPC.h"
#include <UnigineGame.h>
#include <UnigineVisualizer.h>
#include <UnigineRender.h>
#include <UnigineConsole.h>
#include "../../Utils.h"

REGISTER_COMPONENT(VRPlayerPC);

using namespace Unigine;
using namespace Math;

int VRPlayerPC::last_used_control;
int VRPlayerPC::xpad_back = 0;
int VRPlayerPC::xpad_start = 0;

void VRPlayerPC::init()
{
	VRPlayer::init();
	// crouch
	collision_height_max = 1.2f;
	collision_height_min = 0.6f;

	// player
	actor = PlayerActor::cast(node);
	actor->setCollisionRadius(0.3f);
	actor->setCollisionHeight(collision_height_max);
	actor->setMinVelocity(move_speed);
	actor->setMaxVelocity(move_speed * 2.0f);
	actor->setAcceleration(acceleration);
	actor->setPhysicalMass(60.0f);
	actor->setDamping(damping);
	actor->setJumping(1.0f);
	actor->getShape()->setContinuous(1);
	actor->setControlled(1);
	actor->setPhysical(0);
	collision_height = actor->getCollisionHeight();

	saved_dof_blur = Render::get()->getDOFBlur();

	Game::get()->setPlayer(actor->getPlayer());

	head = NodeDummy::create();

	hand = NodeDummy::create();
	hand->setParent(head->getNode());
	hand->setRotation(quat(80, 0, 0));

	// controls
	xpad360 = ControlsXPad360::create(0);
	controls = actor->getControls();
	last_used_control = 0;
	xpad_back = xpad_start = 0;

	last_selected = null_selected;
}

void VRPlayerPC::update()
{
	controls_update();
	zoom_update();
	dof_update();
	crouch_update();
	time_scale_update();
}

void VRPlayerPC::render()
{
	head_update();
	grab_update();
	VRPlayer::render();
}

void VRPlayerPC::shutdown()
{
	VRPlayer::shutdown();
	hand.destroy();
	head.destroy();
	actor.destroy();
}

void VRPlayerPC::setEnabled(int enabled)
{
	//PlayerBase::setEnabled(enabled);

	if (enabled)
	{
		PlayerPtr player = Game::get()->getPlayer();
		if (player)
		{
			actor->setWorldTransform(player->getWorldTransform());

			Vec3 pos = actor->getPosition();
			pos -= actor->getCamera()->getPosition() - pos; // substract height of player
			if (pos.z < 0) pos.z = 0; // check floor

			actor->setPosition(pos);
		}
		Game::get()->setPlayer(getPlayer());
		Render::get()->setFadeColor(vec4::ZERO);
		saved_dof_blur = Render::get()->getDOFBlur();
	}
	else
	{
		Render::get()->setDOFBlur(saved_dof_blur);
	}
}

PlayerPtr VRPlayerPC::getPlayer()
{
	return actor->getPlayer();
}

PlayerActorPtr VRPlayerPC::getActor()
{
	return actor;
}

void VRPlayerPC::disable_outline()
{
	if (last_selected)
	{
		for (int i = 0; i < last_selected->getNumSurfaces(); i++)
			last_selected->setMaterialState("auxiliary", 0, i);

		actor->setPostMaterials("");

		last_selected = null_selected;
	}
}

void VRPlayerPC::controls_update()
{
	App *app = App::get();

	int keyboard_used = 0;
	int mouse_used = 0;
	int gamepad_used = 0;

	int move_left = 0;
	int move_right = 0;
	int backward = 0;
	int forward = 0;
	int turn_left = 0;
	int turn_right = 0;
	int turn_down = 0;
	int turn_up = 0;
	int crouch = 0;
	int jump = 0;
	int run = 0;
	int use = 0;
	int fire = 0;

	// keyboard
	if (!Console::get()->getActivity())
	{
		if (app->getKeyState('a')) { move_left = 1; keyboard_used = 1; }
		if (app->getKeyState('d')) { move_right = 1; keyboard_used = 1; }
		if (app->getKeyState('s')) { backward = 1; keyboard_used = 1; }
		if (app->getKeyState('w')) { forward = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_CTRL)) { crouch = 1; keyboard_used = 1; }
		if (app->clearKeyState(' ')) { jump = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_UP)) { turn_up = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_DOWN)) { turn_down = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_LEFT)) { turn_left = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_RIGHT)) { turn_right = 1; keyboard_used = 1; }
		if (app->getKeyState(App::KEY_SHIFT)) { run = 1; keyboard_used = 1; }
	}

	// mouse
	if (app->getMouseButtonState(App::BUTTON_LEFT)) { use = 1; mouse_used = 1; }
	if (app->getMouseButtonState(App::BUTTON_RIGHT)) { fire = 1; mouse_used = 1; }

	// gamepad
	xpad360->updateEvents();
	if (xpad360->isAvailable())
	{
		float lx = xpad360->getLeftX();
		float ly = xpad360->getLeftY();
		float rx = xpad360->getRightX();
		float ry = xpad360->getRightY();
		float lt = xpad360->getLeftTrigger();
		float rt = xpad360->getRightTrigger();
		float dead_zone = 0.3f;
		float dead_zone_rs = 0.1f; // dead zone for right stick

		if (Math::abs(lx) > dead_zone && lx < 0) { move_left = 1; gamepad_used = 1; }
		if (Math::abs(lx) > dead_zone && lx > 0) { move_right = 1; gamepad_used = 1; }
		if (Math::abs(ly) > dead_zone && ly < 0) { backward = 1; gamepad_used = 1; }
		if (Math::abs(ly) > dead_zone && ly > 0) { forward = 1; gamepad_used = 1; }

		// set slow motion on small values and fast motion on high values
		rx = Math::sign(rx) * (Math::sin(rx * UNIGINE_PI - UNIGINE_PI * 0.5f) + 1.0f) * 0.5f;
		ry = Math::sign(ry) * (Math::sin(ry * UNIGINE_PI - UNIGINE_PI * 0.5f) + 1.0f) * 0.5f;
		// apply zoom effect
		rx = rx * zoom / zoom_max;
		ry = ry * zoom / zoom_max;

		if (Math::abs(rx) > dead_zone_rs) { controls->setMouseDX(rx); gamepad_used = 1; }
		if (Math::abs(ry) > dead_zone_rs) { controls->setMouseDY(-ry); gamepad_used = 1; }

		if (lt > dead_zone) { fire = 1; gamepad_used = 1; }
		if (rt > dead_zone) { use = 1; gamepad_used = 1; }

		if (xpad360->getButton(2)) { crouch = 1; gamepad_used = 1; }
		if (xpad360->clearButton(0)) { jump = 1; gamepad_used = 1; }
		if (xpad360->getButton(6)) { turn_up = 1; gamepad_used = 1; }
		if (xpad360->getButton(7)) { turn_down = 1; gamepad_used = 1; }
		if (xpad360->getButton(8)) { turn_left = 1; gamepad_used = 1; }
		if (xpad360->getButton(9)) { turn_right = 1; gamepad_used = 1; }
		if (xpad360->getButton(10) || xpad360->getButton(12)) { run = 1; gamepad_used = 1; }

		if (xpad360->clearButton(4)) { xpad_back = 1; gamepad_used = 1; }
		else xpad_back = 0;
		if (xpad360->clearButton(5)) { xpad_start = 1; gamepad_used = 1; }
		else xpad_start = 0;
	}

	// apply
	controls->setState(Controls::STATE_MOVE_LEFT, move_left);
	controls->setState(Controls::STATE_MOVE_RIGHT, move_right);
	controls->setState(Controls::STATE_BACKWARD, backward);
	controls->setState(Controls::STATE_FORWARD, forward);
	controls->setState(Controls::STATE_TURN_LEFT, turn_left);
	controls->setState(Controls::STATE_TURN_RIGHT, turn_right);
	controls->setState(Controls::STATE_TURN_DOWN, turn_down);
	controls->setState(Controls::STATE_TURN_UP, turn_up);
	controls->setState(Controls::STATE_CROUCH, crouch);
	controls->setState(Controls::STATE_JUMP, jump);
	controls->setState(Controls::STATE_RUN, run);
	controls->setState(Controls::STATE_USE, use);
	controls->setState(Controls::STATE_FIRE, fire);

	// refresh last used control variable (for interfaces)
	if ((keyboard_used || mouse_used) && last_used_control == 1)
	{
		last_used_control = 0; // keyboard + mouse
	}
	else if (gamepad_used && last_used_control == 0)
	{
		last_used_control = 1; // gamepad
	}
}

void VRPlayerPC::head_update()
{
	head->setPosition(actor->getCamera()->getPosition());
	head->setDirection(actor->getViewDirection(), vec3(0, 0, 1));
}

void VRPlayerPC::grab_update()
{
	// clear key state analogue
	int real_button_pressed = actor->getControls()->getState(Controls::STATE_USE) || (App::get()->getMouseButtonState(App::BUTTON_MIDDLE));
	int button_pressed = real_button_pressed;
	if (last_grab_button_state == button_pressed && button_pressed == 1)
		button_pressed = 0;
	else
		last_grab_button_state = button_pressed;

	// grab update
	if (node_grabbed == 0)
	{
		hand_state = HAND_FREE;

		Vec3 p1 = actor->getCamera()->getPosition();
		Vec3 p2 = p1 + Vec3(actor->getViewDirection()) * hand_length;

		ObjectPtr hit_obj = World::get()->getIntersection(p1, p2, 1, intersection);
		if (hit_obj)
		{
			// all right. It's movable or switcher?
			if (can_i_grab_it(hit_obj->getNode()))
			{
				// show object's outline (auxilary buffer)
				if (last_selected != hit_obj)
				{
					// disable last outline
					disable_outline();

					// enable new outline
					for (int i = 0; i < hit_obj->getNumSurfaces(); i++)
						hit_obj->setMaterialState("auxiliary", 1, i);

					actor->setPostMaterials("vr_post_filter_selection");
				}
				last_selected = hit_obj;

				// grab it, if mouse button pressed
				if (button_pressed)
				{
					grab_node = hit_obj->getNode();
					getComponents<VRInteractable>(grab_node, grab_components);
					node_grabbed = 1;
					hand->setPosition(Vec3(0, 0, -(intersection->getPoint() - p1).length()));
					last_hand_pos = hand->getWorldPosition();
					hand_offset = Vec3::ZERO;
					hand_state = HAND_GRAB;

					// disable all collisions
					// so that the object does not collide with the player
					if (!is_it_handle(grab_node))
						hit_obj->setCollider(0);
				}
				return;
			}
		}
		disable_outline();
	}
	else
	{
		// switchers we can't holding!
		if (is_it_switcher(grab_node))
			button_pressed = 1;

		// handles works as "grab and hold it" instead of "click for grab, then click for release"
		if (is_it_handle(grab_node) && !real_button_pressed)
			button_pressed = 1;

		// throw it, if mouse button pressed
		if (button_pressed)
		{
			// enable all collisions
			if (!is_it_handle(grab_node))
				grab_node->setCollider(1);

			node_grabbed = 0;
			hand_state = HAND_THROW;
		}
		// or, hold this
		else
		{
			hand_state = HAND_HOLD;
			hand_offset = hand->getWorldPosition() - last_hand_pos;
			last_hand_pos = hand->getWorldPosition();
		}
	}
}

void VRPlayerPC::zoom_update()
{
	float ifps = Game::get()->getIFps() / Game::get()->getScale();
	zoom = lerp_clamped(zoom, actor->getControls()->getState(Controls::STATE_FIRE) ? zoom_min : zoom_max, zoom_factor * ifps);
	actor->setFov(zoom);
	ControlsApp::get()->setMouseSensitivity(mouse_sensitivity * zoom / zoom_max);
}

void VRPlayerPC::dof_update()
{
	Vec3 p1 = head->getPosition();
	Vec3 p2 = p1 + Vec3(head->getDirection()) * actor->getZFar();

	ObjectPtr hit_obj = World::get()->getIntersection(p1, p2, 1, intersection);
	if (hit_obj)
		dof_dist = lerp_clamped(dof_dist, (float)(intersection->getPoint() - p1).length(), dof_dist_factor * Game::get()->getIFps() / Game::get()->getScale());

	float z = zoom / zoom_max;
	float dof_blur = saturate(1.0f - ((zoom - zoom_min) / (zoom_max - zoom_min)));
	if (dof_blur < 0.01f) dof_blur = 0;

	Render::get()->setDOFBlur(dof_blur);
	Render::get()->setDOFFocalDistance(dof_dist);
	Render::get()->setDOFNearDistance(lerp(dof_min_dist, dof_max_dist, dof_dist / actor->getZFar()) * z);
	Render::get()->setDOFFarDistance(lerp(dof_min_dist, dof_max_dist, dof_dist / actor->getZFar()) * z);
}

void VRPlayerPC::crouch_update()
{
	if (actor->getControls()->getState(Controls::STATE_CROUCH))
		collision_height = lerp(collision_height, collision_height_min, 10.0f * Game::get()->getIFps() / Game::get()->getScale());
	else
		collision_height = lerp(collision_height, collision_height_max, 10.0f * Game::get()->getIFps() / Game::get()->getScale());

	Vec3 pos = actor->getPosition();
	actor->setCollisionHeight(collision_height);
	actor->setTransform(translate(pos) * rotation(actor->getCamera()->getIModelview()));
}

void VRPlayerPC::time_scale_update()
{
	float scale = Game::get()->getScale();
	actor->setMinVelocity(move_speed / scale);
	actor->setMaxVelocity(move_speed * 2.0f / scale);
	actor->setAcceleration(acceleration / scale);
	actor->setDamping(damping / scale);
}

int VRPlayerPC::getNumHands()
{
	return 1;
}

NodePtr VRPlayerPC::getHandNode(int num)
{
	return hand->getNode();
}

int VRPlayerPC::getHandDegreesOfFreedom(int num)
{
	return 0;
}

vec3 VRPlayerPC::getHandLinearVelocity(int num)
{
	if (hand_state == HAND_THROW)
	{
		if (App::get()->getMouseButtonState(App::BUTTON_MIDDLE))
			return vec3::ZERO;
		else
			return actor->getViewDirection() * throw_force;
	}
	else
		return vec3(hand_offset * 0.1f * Game::get()->getScale() / Game::get()->getIFps());
}

vec3 VRPlayerPC::getHandAngularVelocity(int num)
{
	if (hand_state == HAND_THROW)
	{
		if (App::get()->getMouseButtonState(App::BUTTON_MIDDLE))
			return vec3::ZERO;
		else
		{
			Game *game = Game::get();
			float x = game->getRandomFloat(-1.0f, 1.0f);
			float y = game->getRandomFloat(-1.0f, 1.0f);
			float z = game->getRandomFloat(-1.0f, 1.0f);

			return vec3(x, y, z) * throw_force;
		}
	}
	else
		return vec3::ZERO;
}

int VRPlayerPC::getHandState(int num)
{
	return hand_state;
}

const NodePtr &VRPlayerPC::getGrabNode(int num) const
{
	return grab_node;
}

const Vector<VRInteractable*> &VRPlayerPC::getGrabComponents(int num) const
{
	return grab_components;
}

int VRPlayerPC::getControllerButton(int controller_num, int button)
{
	if (Console::get()->getActivity())
		return 0;

	switch (button)
	{
	case STICK_X: return App::get()->getKeyState(App::KEY_LEFT) || App::get()->getKeyState(App::KEY_RIGHT);
	case STICK_Y: return App::get()->getKeyState(App::KEY_UP) || App::get()->getKeyState(App::KEY_DOWN);
	case TRIGGER: return App::get()->getKeyState('f');
	case GRIP: return App::get()->getMouseButtonState(0);
	case XA: return App::get()->getKeyState('z');
	case YB: return App::get()->getKeyState('q');
	case MENU: return App::get()->getMouseButtonState(App::KEY_TAB);
	}

	return 0;
}

int VRPlayerPC::getControllerButtonDown(int controller_num, int button)
{
	return getControllerButton(controller_num, button) && !buttons_prev_state[button];
}

int VRPlayerPC::getControllerButtonUp(int controller_num, int button)
{
	return !getControllerButton(controller_num, button) && buttons_prev_state[button];
}

float VRPlayerPC::getControllerAxis(int controller_num, int button)
{
	if (Console::get()->getActivity())
		return 0;

	switch (button)
	{
	case STICK_X: return itof(App::get()->getKeyState(App::KEY_RIGHT) - App::get()->getKeyState(App::KEY_LEFT));
	case STICK_Y: return itof(App::get()->getKeyState(App::KEY_UP) - App::get()->getKeyState(App::KEY_DOWN));
	case TRIGGER: return itof(App::get()->getKeyState('f'));
	case GRIP: return itof(App::get()->getMouseButtonState(0));
	case XA: return itof(App::get()->getKeyState('z'));
	case YB: return itof(App::get()->getKeyState('q'));
	case MENU: return itof(App::get()->getMouseButtonState(App::KEY_TAB));
	}

	return 0;
}

void VRPlayerPC::update_button_states()
{
	for (int i = 0; i < BUTTON::COUNT; i++)
		buttons_prev_state[i] = getControllerButton(0, i);
}

void VRPlayerPC::vibrateController(int controller_num, float amplitude)
{
	/*
	if (xpad360->isAvailable())
	{
	if (controller_num == 0) xpad360->setLeftMotor(amplitude);
	if (controller_num == 1) xpad360->setRightMotor(amplitude);
	}
	*/
}
