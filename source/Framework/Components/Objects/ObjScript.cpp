#include <UnigineInterface.h>

#include "ObjScript.h"
#include "../../Utils.h"

REGISTER_COMPONENT(ObjScript);

using namespace Unigine;
using namespace Math;

Unigine::VectorStack<ObjScript *, 128> ObjScript::scripts;

static int function_arguments[] = {
	0, 0, 0,
	1, 0, 0,
	1, 1, 1, 1,
};

static const char *function_names[] = {
	"hasHandleBehaviour", "hasSwitcherBehaviour", "getClass",
	"init", "update", "shutdown",
	"grabIt", "holdIt", "useIt", "throwIt",
};

void ObjScript::init()
{
	assert(scripts.find(this) == scripts.end());
	scripts.append(this);

	Engine *engine = Engine::get();
	assert(engine != nullptr);

	reload();
}

void ObjScript::update()
{
	call_script(update_func_id);
}

void ObjScript::shutdown()
{
	auto it = scripts.find(this);
	assert(it != scripts.end());

	scripts.removeFast(it);

	cleanup();
}

bool ObjScript::reload()
{
	Engine *engine = Engine::get();
	assert(engine != nullptr);

	Expression *new_expression = new Expression(engine->getWorldInterpreter(), String::format("{\n#include <%s>\n}", script.get()), 1);
	if (!new_expression->isCompiled())
	{
		delete new_expression;
		return false;
	}

	cleanup();
	expression = new_expression;

	const size_t num_functions = 10;

	int *function_ids[] = {
		&has_handle_behaviour_func_id, &has_switcher_behaviour_func_id, &get_class_func_id,
		&init_func_id, &update_func_id, &shutdown_func_id,
		&grab_func_id, &hold_func_id, &use_func_id, &throw_func_id,
	};

	for (size_t i = 0; i < num_functions; i++)
	{
		if (!expression->isFunction(function_names[i], function_arguments[i]))
			continue;

		*function_ids[i] = expression->getFunction(function_names[i], function_arguments[i]);
	}

	Variable v;
	v.setExternClassObject(engine->getWorldInterpreter(), this);
	call_script(init_func_id, v);

	return true;
}

bool ObjScript::hasSwitcherBehaviour() const
{
	Variable ret = call_script(has_switcher_behaviour_func_id);

	if (ret.isInt())
		return ret.getInt() != 0;

	return false;
}

bool ObjScript::hasHandleBehaviour() const
{
	Variable ret = call_script(has_handle_behaviour_func_id);

	if (ret.isInt())
		return ret.getInt() != 0;

	return false;
}

Unigine::Variable ObjScript::getScriptClass() const
{
	return call_script(get_class_func_id);
}

Variable ObjScript::call_script(int func) const
{
	Engine *engine = Engine::get();
	assert(engine != nullptr);

	Variable ret;
	if (func != 0)
		ret = engine->runWorldFunction(Variable(func));

	return ret;
}

Variable ObjScript::call_script(int func, const Variable &argument) const
{
	Engine *engine = Engine::get();
	assert(engine != nullptr);

	Variable ret;
	if (func != 0)
		ret = engine->runWorldFunction(Variable(func), argument);

	return ret;
}

void ObjScript::cleanup()
{
	call_script(shutdown_func_id);

	delete expression;
	expression = nullptr;

	has_switcher_behaviour_func_id = 0;
	has_handle_behaviour_func_id = 0;
	get_class_func_id = 0;
	init_func_id = 0;
	update_func_id = 0;
	shutdown_func_id = 0;
	grab_func_id = 0;
	hold_func_id = 0;
	use_func_id = 0;
	throw_func_id = 0;
}

void ObjScript::grabIt(VRPlayer *player, int hand_num)
{
	call_script(grab_func_id, Variable(hand_num));
}

void ObjScript::holdIt(VRPlayer *player, int hand_num)
{
	call_script(hold_func_id, Variable(hand_num));
}

void ObjScript::useIt(VRPlayer *player, int hand_num)
{
	call_script(use_func_id, Variable(hand_num));
}

void ObjScript::throwIt(VRPlayer *player, int hand_num)
{
	call_script(throw_func_id, Variable(hand_num));
}

void ObjScript::initInterpreter()
{
	ExternClass<ObjScript> *obj_script = MakeExternClass<ObjScript>();

	obj_script->addFunction("getNode", &ObjScript::getNode);
	obj_script->addFunction("getScriptName", &ObjScript::getScriptName);
	obj_script->addFunction("getScriptExpression", &ObjScript::getScriptExpression);
	obj_script->addFunction("getScriptClass", &ObjScript::getScriptClass);
	obj_script->addFunction("reload", &ObjScript::reload);

	Interpreter::addExternClass("ObjScript", obj_script);

	Interpreter::addExternLibrary("scripts");
	Interpreter::addExternFunction("scripts.getNumHands", MakeExternFunction(&ObjScript::getNumHands));
	Interpreter::addExternFunction("scripts.getHandNode", MakeExternFunction(&ObjScript::getHandNode));
	Interpreter::addExternFunction("scripts.getScriptClassByNodeName", MakeExternFunction(&ObjScript::getScriptClassByNodeName));
	Interpreter::addExternFunction("scripts.getScriptByNodeName", MakeExternFunction(&ObjScript::getScriptByNodeName));
	Interpreter::addExternFunction("scripts.getGui", MakeExternFunction(&ObjScript::getGui));
	Interpreter::addExternFunction("scripts.getNumScripts", MakeExternFunction(&ObjScript::getNumScripts));
	Interpreter::addExternFunction("scripts.getScript", MakeExternFunction(&ObjScript::getScript));
}

void ObjScript::shutdownInterpreter()
{
	Interpreter::removeExternClass("ObjScript");

	Interpreter::removeExternFunction("scripts.getNumHands");
	Interpreter::removeExternFunction("scripts.getHandNode");
	Interpreter::removeExternFunction("scripts.getScriptClassByNodeName");
	Interpreter::removeExternFunction("scripts.getScriptByNodeName");
	Interpreter::removeExternFunction("scripts.getGui");
	Interpreter::removeExternFunction("scripts.getNumScripts");
	Interpreter::removeExternFunction("scripts.getScript");
	Interpreter::removeExternLibrary("scripts");
}

int ObjScript::getNumHands()
{
	VRPlayer *vr_player = VRPlayer::get();
	if (vr_player && vr_player->isInitialized())
		return vr_player->getNumHands();

	return -1;
}

NodePtr ObjScript::getHandNode(int index)
{
	VRPlayer *vr_player = VRPlayer::get();
	if (!vr_player || !vr_player->isInitialized())
		return NodePtr();

	if (index < 0 || index >= vr_player->getNumHands())
		return NodePtr();

	return vr_player->getHandNode(index);
}

Variable ObjScript::getScriptByNodeName(const char *name)
{
	Editor *editor = Editor::get();
	assert(editor != nullptr);

	Engine *engine = Engine::get();
	assert(engine != nullptr);

	NodePtr node = editor->getNodeByName(name);

	Variable ret;
	if (node.get() != nullptr)
	{
		ComponentSystem *components = ComponentSystem::get();
		ObjScript *script = components->getComponentInChildren<ObjScript>(node);
		ret.setExternClassObject(engine->getWorldInterpreter(), script);
	}

	return ret;
}

Variable ObjScript::getScriptClassByNodeName(const char *name)
{
	Editor *editor = Editor::get();
	assert(editor!= nullptr);

	NodePtr node = editor->getNodeByName(name);

	Variable ret;
	if (node.get() != nullptr)
	{
		ComponentSystem *components = ComponentSystem::get();
		ObjScript *script = components->getComponentInChildren<ObjScript>(node);
		ret = script->getScriptClass();
	}

	return ret;
}

GuiPtr ObjScript::getGui()
{
	VRPlayer *vr_player = VRPlayer::get();
	if (vr_player)
	{
		if (vr_player->isInitialized())
			return vr_player->getGui();
		else
			return GuiPtr();
	}

	return Gui::get();
}

int ObjScript::getNumScripts()
{
	return scripts.size();
}

ObjScript *ObjScript::getScript(int index)
{
	if (index < 0 || index >= scripts.size())
		return nullptr;

	return scripts[index];
}
