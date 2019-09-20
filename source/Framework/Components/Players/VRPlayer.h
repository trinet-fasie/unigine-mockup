#pragma once
#include <UnigineNode.h>
#include <UnigineNodes.h>
#include <UnigineWorld.h>
#include <UniginePlayers.h>
#include <UniginePhysics.h>
#include <UnigineVector.h>
#include <UnigineGui.h>
#include <UnigineMap.h>
#include <UnigineGame.h>

#include "../../../ComponentSystem/ComponentSystem.h"
#include "../VRInteractable.h"

class VRInteractable;

class VRPlayer : public ComponentBase
{
public:
	COMPONENT(VRPlayer, ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_RENDER(render);
	COMPONENT_SHUTDOWN(shutdown);

	///////////////////////////////////////
	// enums
	///////////////////////////////////////

	enum {
		HAND_FREE,
		HAND_GRAB,
		HAND_HOLD,
		HAND_THROW
	};

	enum GRAB_MODE
	{
		BOUNDBOX,
		INTERSECTIONS
	};

	// baseline controls for VR Controllers (Vive, Oculus, XBox):
	// http://metanautvr.com/wp-content/uploads/2017/07/VRControllersBaselineComparison2C.png
	enum BUTTON
	{
		STICK_X,
		STICK_Y,
		TRIGGER,
		GRIP,
		XA, // X or A
		YB, // Y or B
		MENU,

		COUNT, // reserved. buttons count
	};

	///////////////////////////////////////
	// static
	///////////////////////////////////////

	// is -extern_plugin "AppVive/AppOculus" added to command line and successfully loaded?
	static int isVRPluginLoaded();

	// return pointer to last created PlayerVR
	static VRPlayer* get();

	///////////////////////////////////////
	// methods
	///////////////////////////////////////
	
	// sets intersection mask for teleportation (where player can be)
	virtual void setTeleportationMask(int mask) {}

	// player
	virtual Unigine::PlayerPtr getPlayer() { return Unigine::Game::get()->getPlayer(); }
	virtual void setLock(int lock) {}
	virtual void setPlayerPosition(const Unigine::Math::Vec3 &pos);

	// move player to position (and then land him to ground) with some direction
	virtual void landPlayerTo(const Unigine::Math::Vec3 &position, const Unigine::Math::vec3 &direction);
	UNIGINE_INLINE virtual void landPlayerTo(const Unigine::Math::Mat4 &transform)
	{
		landPlayerTo(transform.getColumn3(3), normalize(Unigine::Math::vec3(-transform.getColumn3(2))));
	}

	// head
	UNIGINE_INLINE virtual Unigine::NodePtr getHead() { return head->getNode(); }
	
	// hands
	virtual void setGrabMode(GRAB_MODE mode) { grab_mode = mode; } // grab via BoundBox-BoundBox or Line-Surface intersection?
	virtual int getNumHands() { return 0; }						// get count of hands
	virtual Unigine::NodePtr getHandNode(int num) = 0;			// get hand's node
	virtual int getHandDegreesOfFreedom(int num) = 0;	// get hand's degrees of freedom (0-5 for PC, 6 for vive)
	virtual Unigine::Math::vec3 getHandLinearVelocity(int num) = 0;	// get speed of hand
	virtual Unigine::Math::vec3 getHandAngularVelocity(int num) = 0;	// get angular speed of hand
	virtual int getHandState(int num) = 0;				// get hand state (grab, hold, throw)
	virtual const Unigine::NodePtr &getGrabNode(int num) const = 0;			// get object, grabbed by hand
	virtual const Unigine::Vector<VRInteractable*> &getGrabComponents(int num) const = 0;
	UNIGINE_INLINE virtual Unigine::Math::vec3 getHandyPos() { return Unigine::Math::vec3(0, -0.024f, 0.067f); }
	UNIGINE_INLINE virtual Unigine::Math::quat getHandyRot() { return Unigine::Math::quat(10.0f, 0, 0); }

	// hand's controllers
	virtual int getControllerButton(int controller_num, int button) = 0;
	virtual int getControllerButtonDown(int controller_num, int button) = 0;
	virtual int getControllerButtonUp(int controller_num, int button) = 0;
	virtual float getControllerAxis(int controller_num, int button) = 0;
	virtual void vibrateController(int controller_num, float amplitude = 1.0f) = 0;

	// gui
	UNIGINE_INLINE virtual Unigine::GuiPtr getGui() { return Unigine::Gui::get(); }

	// callbacks
	// called when player holds "use" button when holding some object
	UNIGINE_INLINE void setUseCallback(Unigine::CallbackBase* callback) { delete use_callback; use_callback = callback; } // (NodePtr node)
	// called when player grab some object
	UNIGINE_INLINE void setGrabCallback(Unigine::CallbackBase* callback) { delete grab_callback; grab_callback = callback; } // (NodePtr node)
	// called when player holding some object
	UNIGINE_INLINE void setHoldCallback(Unigine::CallbackBase* callback) { delete hold_callback; hold_callback = callback; } // (NodePtr node)
	// called when player throw some object
	UNIGINE_INLINE void setThrowCallback(Unigine::CallbackBase* callback) { delete throw_callback; throw_callback = callback; } // (NodePtr node)

protected:
	void init();
	void render();
	void shutdown();

	virtual void update_button_states() = 0;

	// find specific components on the node
	int can_i_grab_it(const Unigine::NodePtr &node);
	int is_it_switcher(const Unigine::NodePtr &node);
	int is_it_handle(const Unigine::NodePtr &node);

	Unigine::NodeDummyPtr head;
	GRAB_MODE grab_mode = GRAB_MODE::BOUNDBOX;

private:
	// singleton
	static VRPlayer* instance;

	// callbacks
	Unigine::CallbackBase* use_callback;
	Unigine::CallbackBase* grab_callback;
	Unigine::CallbackBase* hold_callback;
	Unigine::CallbackBase* throw_callback;
};
