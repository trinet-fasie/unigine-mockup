#pragma once

#include <UnigineWorld.h>
#include <UnigineInterpreter.h>
#include "../VRInteractable.h"

class ObjScript : public VRInteractable
{
public:
	COMPONENT(ObjScript, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);

	// property
	PROP_NAME("script");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(File, script);

	inline const char *getScriptName() const { return script.get(); }
	inline Unigine::Expression *getScriptExpression() const { return expression; }

	Unigine::Variable getScriptClass() const;

	bool reload();

	// behaviour methods
	bool hasSwitcherBehaviour() const;
	bool hasHandleBehaviour() const;

	// interact methods
	void grabIt(VRPlayer* player, int hand_num) override;
	void holdIt(VRPlayer* player, int hand_num) override;
	void useIt(VRPlayer* player, int hand_num) override;
	void throwIt(VRPlayer* player, int hand_num) override;

	static void initInterpreter();
	static void shutdownInterpreter();

	static int getNumHands();
	static Unigine::NodePtr getHandNode(int index);
	static Unigine::Variable getScriptClassByNodeName(const char *name);
	static Unigine::Variable getScriptByNodeName(const char *name);
	static Unigine::GuiPtr getGui();
	static int getNumScripts();
	static ObjScript *getScript(int index);

protected:
	void init();
	void update();
	void shutdown();

	Unigine::Variable call_script(int func) const;
	Unigine::Variable call_script(int func, const Unigine::Variable &argument) const;
	void cleanup();

private:
	Unigine::Expression *expression {nullptr};
	int has_switcher_behaviour_func_id {0};
	int has_handle_behaviour_func_id {0};
	int get_class_func_id {0};
	int init_func_id {0};
	int update_func_id {0};
	int shutdown_func_id {0};
	int grab_func_id {0};
	int hold_func_id {0};
	int use_func_id {0};
	int throw_func_id {0};

	static Unigine::VectorStack<ObjScript *, 128> scripts;
};
