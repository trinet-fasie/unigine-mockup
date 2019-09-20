#include "AppSystemLogic.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Framework/Components/Objects/ObjScript.h"

int AppSystemLogic::init()
{
	ObjScript::initInterpreter();

	// run ComponentSystem
	ComponentSystem::get()->initialize();

	// show all warning/error messages to the log
	ComponentSystem::get()->setWarningLevel(ComponentSystem::WARNING_LEVEL::HIGH);

	return 1;
}

int AppSystemLogic::shutdown()
{
	ObjScript::shutdownInterpreter();

	return 1;
}
