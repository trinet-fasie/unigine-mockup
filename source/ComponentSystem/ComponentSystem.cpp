#include "ComponentSystem.h"
#include <UnigineGame.h>
#include <UnigineEditor.h>
#include <UnigineEngine.h>
#include <UnigineApp.h>
#include <UnigineProperties.h>
#include <UnigineDir.h>
#include <UnigineStreams.h>
#include <UnigineProfiler.h>
#include <UnigineTimer.h>
#include <UnigineConsole.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

using namespace Unigine;
using namespace Math;

////////////////////////////////////////////////////////////////////////////////////////
// Thread Pool
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region ThreadPool
#endif

ThreadPoolWorker::ThreadPoolWorker(ThreadPool *thread_pool)
	: tp(thread_pool)
{
	run();
}

void ThreadPoolWorker::process()
{
	while (true)
	{
		std::unique_lock<std::mutex> latch(tp->queue_mutex);
		tp->cv_task.wait(latch, [this]()
		{
			return tp->stop || !tp->tasks.empty();
		});
		if (!tp->tasks.empty())
		{
			// got work. set busy.
			++tp->busy;

			// pull from queue
			auto fn = tp->tasks.front();
			tp->tasks.pop_front();

			// release lock. run async
			latch.unlock();

			// run function outside context
			fn();
			++tp->processed;

			latch.lock();
			--tp->busy;
			tp->cv_finished.notify_one();
		}
		else if (tp->stop)
			break;
	}
}

ThreadPool::ThreadPool(unsigned int n)
	: processed(), busy(), stop()
{
	for (unsigned int i = 0; i<n; ++i)
		workers.emplace_back(new ThreadPoolWorker(this));
}

ThreadPool::~ThreadPool()
{
	// set stop-condition
	std::unique_lock<std::mutex> latch(queue_mutex);
	stop = true;
	cv_task.notify_all();
	latch.unlock();

	// all threads terminate, then we're done.
	for (auto& t : workers)
	{
		t->stop();
		delete t;
	}
}

// waits until the queue is empty.
void ThreadPool::waitFinished()
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	cv_finished.wait(lock, [this]() { return tasks.empty() && (busy == 0); });
}

#ifndef __GNUC__
#pragma endregion ThreadPool
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component System
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region System
#endif

ComponentSystem::ComponentSystem()
	: callback_func_add(nullptr)
	, callback_func_swap(nullptr)
	, callback_func_remove(nullptr)
	, initialized(0)
	, shutdowned(0)
	, profiler_time(0)
	, warning_level(WARNING_LEVEL::LOW)
	, threads(nullptr)
{
}

ComponentSystem::~ComponentSystem()
{
	if (!initialized)
		return;

	// clear components
	for (auto i = components.begin(); i != components.end(); ++i)
		for (int k = 0; k < i->data.size(); k++)
			delete i->data[k];
	components.clear();
}

void ComponentSystem::initialize()
{
	// run callbacks before initialization
	// (usually we call ComponentSystem::get()->registerComponent<>() here)
	for (int i = 0; i < initialize_callbacks.size(); i++)
	{
		initialize_callbacks[i]->run();
		delete initialize_callbacks[i];
	}
	initialize_callbacks.clear();

	// modify property files if needed
	createPropertyFiles();

	// don't subscribe or addWorldLogic twice
	if (initialized)
		return;

	// subscribe to create nodes with properties
	callback_func_add = Node::addCallback(Node::CALLBACK_PROPERTY_NODE_ADD, MakeCallback(this, &ComponentSystem::property_created));
	callback_func_swap = Node::addCallback(Node::CALLBACK_PROPERTY_NODE_SWAP, MakeCallback(this, &ComponentSystem::property_swapped));
	callback_func_remove = Node::addCallback(Node::CALLBACK_PROPERTY_NODE_REMOVE, MakeCallback(this, &ComponentSystem::property_removed));

	Engine::get()->addWorldLogic(this);

	initialized = 1;
}

void ComponentSystem::addInitCallback(Unigine::CallbackBase *callback)
{
	initialize_callbacks.append(callback);
}

void ComponentSystem::createPropertyFiles()
{
	// create instances
	NodePtr null_node;
	Vector<String> names;
	Vector<ComponentBase*> c;
	component_factory.getComponentNames(names);
	for (int i = 0; i < names.size(); i++)
		c.append(component_factory.create(names[i], null_node, 0));

	for (int i = 0; i < c.size(); i++)
		if (c[i]->isAutoSaveProperty())
			c[i]->save_property(names[i].get());

	// delete instances
	for (int i = 0; i < c.size(); i++)
		delete c[i];
}

void ComponentSystem::createPropertyFile(const char *name)
{
	// create instance
	ComponentBase *c = component_factory.create(name, NodePtr(), 0);

	// make file and directory
	c->save_property(name);

	// delete instance
	delete c;
}

void ComponentSystem::refreshProperty(const char *name)
{
	// rewrite property file
	PropertyPtr prop = Properties::get()->findManualProperty(name);
	if (prop)
	{
		ComponentBase *c = component_factory.create(name, NodePtr(), 0);
		c->save_property(name); 
		delete c;
	}

	// reload property
	prop->reload();
}

int ComponentSystem::getNumComponents()
{
	int count = 0;
	for (auto i = components.begin(); i != components.end(); ++i)
		for (int k = 0; k < i->data.size(); k++)
			if (i->data[k])
				count++;

	return count;
}

#define METHOD_REGISTRATOR(METHOD, METHOD_LOWCASE) \
void ComponentSystem::addComponentMethod##METHOD(ComponentBase *component, CallbackBase *func, const char *name, const char *func_name, int order)		\
{																																						\
	/* check on existence */																															\
	for (int i = 0; i < component_functions_##METHOD_LOWCASE[order].size(); i++)																		\
	{																																					\
		auto &it = component_functions_##METHOD_LOWCASE[order][i];																						\
		if (it.component == component && !strcmp(it.func_name, func_name))																				\
		{																																				\
			/* this method overrides existed one */																										\
			for (int k = 0; k < component->functions_##METHOD_LOWCASE[order].size(); k++)																\
				if (component->functions_##METHOD_LOWCASE[order][k] == it.function)																		\
				{																																		\
					component->functions_##METHOD_LOWCASE[order][k] = func;																				\
					break;																																\
				}																																		\
																																						\
			it.function = func;																															\
			it.name = name;																																\
			return;																																		\
		}																																				\
	}																																					\
																																						\
	/* its a new method */																																\
	component_functions_##METHOD_LOWCASE[order].append({ component, func, name, func_name, false });													\
																																						\
	/* used in shutdown() before destroy a node */																										\
	component->functions_##METHOD_LOWCASE[order].append(func);																							\
}																																						\
void ComponentSystem::removeComponentMethod##METHOD(ComponentBase *component, CallbackBase *func, const char *name, const char *func_name, int order)	\
{																																						\
	auto &vector = component_functions_##METHOD_LOWCASE[order];																							\
	auto it = vector.find<ComponentCallback>({ component, func, name, func_name, false });																\
	if (it != vector.end())																																\
	{																																					\
		vector.remove(it);																																\
		if (!vector.size())																																\
			component_functions_##METHOD_LOWCASE.remove(order);																							\
	}																																					\
}
METHOD_REGISTRATOR(Init, init);
METHOD_REGISTRATOR(Update, update);
METHOD_REGISTRATOR(ParallelUpdate, parallelUpdate);
METHOD_REGISTRATOR(Render, render);
METHOD_REGISTRATOR(Flush, flush);
METHOD_REGISTRATOR(Shutdown, shutdown);
METHOD_REGISTRATOR(Destroy, destroy);
#undef METHOD_REGISTRATOR

// call some function in all components
#define CALL_COMPONENT_FUNC(NAME, OTHER_CODE)														\
	for (auto i = component_functions_##NAME.begin(); i != component_functions_##NAME.end(); ++i)	\
		for (int k = 0; k < i->data.size(); k++)													\
		{																							\
			ComponentCallback &cc = i->data[k];														\
			OTHER_CODE																				\
		}

// check isEnabled() flag
#define GET_ENABLED_VAR() ComponentBase *c = cc.component; int is_enabled = c->isEnabled();

// call on_enable() / on_disable() methods
#define CALL_ON_ENABLE_FUNC()										\
	if (c->prev_enabled != is_enabled)								\
	{																\
		c->prev_enabled = is_enabled;								\
		if (c->all_init_called)										\
		{															\
			if (is_enabled)											\
				c->on_enable();										\
			else													\
				c->on_disable();									\
		}															\
	}

// call some method
#ifdef USE_MICROPROFILE
#define CALL_FUNC(PROFILER) int pid = PROFILER::get()->beginMicro(cc.name); cc.function->run(); PROFILER::get()->endMicro(pid);
#else
#define CALL_FUNC(PROFILER) cc.function->run();
#endif

// call some method (only if it enabled and already initialized)
#define CALL_FUNC_IF_ENABLED()				\
	if (is_enabled && c->all_init_called)	\
	{										\
		CALL_FUNC(Profiler)					\
	}

void ComponentSystem::run_init_methods()
{
	VectorStack<int> need_to_remove;

	// for "order min value" to "order max value" for "init" method
	for (auto i = component_functions_init.begin(); i != component_functions_init.end(); ++i)
	{
		int count_of_calls = 0;

		// for components on a specific order
		for (int k = 0; k < i->data.size(); k++)
		{
			ComponentCallback &cc = i->data[k];
			ComponentBase *c = cc.component;
			
			if (!c->isEnabled())
				continue;

			// initialize component (before call init() method)
			// show errors, do some things with node...
			if (!c->initialized)
			{
				c->null_check();
				c->initialize();
			}

			// run init() method
			if (!cc.called)
			{
				cc.called = true;
				c->init_called();
				count_of_calls++;

				// warning: after call run() function below don't do anything with
				// the "cc" variable! Vector i->data might be reallocated (moved).
				// it can happens when component creates another component in
				// its init() function

#ifdef USE_MICROPROFILE
				int pid = Profiler::get()->beginMicro(cc.name);
				cc.function->run();
				Profiler::get()->endMicro(pid);
#else
				cc.function->run();
#endif
			}
			else
				count_of_calls++;
		}

		// can we remove this order?
		// all of the components ran its init() methods at this order?
		if (count_of_calls == i->data.size())
		{
			i->data.clear();
			need_to_remove.append(i->key);
		}
	}

	// clear unnecessary orders
	for (int i = 0; i < need_to_remove.size(); i++)
		component_functions_init.remove(need_to_remove[i]);
}

int ComponentSystem::init()
{
	shutdowned = 0;
	
	// init thread pool for parallelUpdate
	threads = new ThreadPool();

	// call init() method (for components that created at design time)
	run_init_methods();
	
	destroy_nodes();
	return 1;
}

int ComponentSystem::update()
{
#ifdef USE_MICROPROFILE
	int puid = Profiler::get()->beginMicro(__FUNCTION__);
#endif
	long long update_time = Timer::getTime();
	
	// call init() method (for components that created at run time)
	run_init_methods();

	// 1) call on_enable() / on_disable() methods (if needed)
	// 2) call update() method
	CALL_COMPONENT_FUNC(update, GET_ENABLED_VAR(); CALL_ON_ENABLE_FUNC(); CALL_FUNC_IF_ENABLED());
	
	// destroy all queued nodes (after call destroyNode())
	destroy_nodes();
	
	profiler_time += Timer::getTime() - update_time;
#ifdef USE_MICROPROFILE
	Profiler::get()->endMicro(puid);
#endif
	return 1;
}

int ComponentSystem::render()
{
#ifdef USE_MICROPROFILE
	int prid = Profiler::get()->beginMicro(__FUNCTION__);
#endif
	long long render_time = Timer::getTime();

	// parallel update
#ifndef __GNUC__
#pragma region parallelUpdate
#endif
	for (auto i = component_functions_parallelUpdate.begin(); i != component_functions_parallelUpdate.end(); ++i)
	{
		int workers = threads->getNumWorkers();
		int block_size = i->data.size() / workers;

		for (int k = 0; k < workers; k++)
		{
			threads->enqueue([this, i, k, block_size]
			{
				int from = k * block_size;
				int to = from + block_size;

				for (int z = from; z < to; z++)
				{
					ComponentCallback &cc = i->data[z];
					ComponentBase *c = cc.component;
					if (!c->all_init_called)
						continue;

					if (c->isEnabled())
					{
#ifdef USE_MICROPROFILE
						int pid = Profiler::get()->beginMicro(cc.name);
#endif
						cc.function->run(); // call parallelUpdate function
#ifdef USE_MICROPROFILE
						Profiler::get()->endMicro(pid);
#endif
					}
				}
			});
		}

		threads->waitFinished();
	}
#ifndef __GNUC__
#pragma endregion parallelUpdate
#endif

	// render
	CALL_COMPONENT_FUNC(render, GET_ENABLED_VAR(); CALL_FUNC_IF_ENABLED());
	destroy_nodes();

	profiler_time += Timer::getTime() - render_time;
	render_profiler();

#ifdef USE_MICROPROFILE
	Profiler::get()->endMicro(prid);
#endif
	return 1;
}

int ComponentSystem::flush()
{
#ifdef USE_MICROPROFILE
	int pfid = Profiler::get()->beginMicro(__FUNCTION__);
#endif
	long long flush_time = Timer::getTime();

	CALL_COMPONENT_FUNC(flush, GET_ENABLED_VAR(); CALL_FUNC_IF_ENABLED());

	profiler_time += Timer::getTime() - flush_time;
#ifdef USE_MICROPROFILE
	Profiler::get()->endMicro(pfid);
#endif
	return 1;
}

int ComponentSystem::shutdown()
{
	shutdowned = 1;
	CALL_COMPONENT_FUNC(shutdown, CALL_FUNC(Profiler));

	// destroy all components and their methods
	for (auto i = components.begin(); i != components.end(); ++i)
		for (int k = 0; k < i->data.size(); k++)
			delete i->data[k];
	components.clear();
	component_functions_init.clear();
	component_functions_update.clear();
	component_functions_parallelUpdate.clear();
	component_functions_render.clear();
	component_functions_flush.clear();
	component_functions_shutdown.clear();
	component_functions_destroy.clear();

	destroy_nodes();
	delete threads;
	return 1;
}

int ComponentSystem::destroy()
{
	CALL_COMPONENT_FUNC(destroy, CALL_FUNC(Profiler));
	return 1;
}

void ComponentSystem::property_created(const NodePtr node, const PropertyPtr prop, int num)
{
	// remove duplicates
	auto it = components.find(node->getID());
	if (it != components.end() && it->data.size() > num &&
		it->data[num] && it->data[num]->property == prop)
	{
		property_removed(node, prop, num);
	}

	// create component for created property
	ComponentBase *c = nullptr;
	PropertyPtr p = prop;
	while (!c && p)
	{
		c = component_factory.create(p->getName(), node, num);
		p = p->getParent();
	}
	if (c)
		c->node_id = node->getID();

	// put the new component in Vector<ComponentBase*>
	// in the same index as the "num" variable
	auto &list = components[node->getID()];

	// reserve
	int prev_size = list.size();
	int need_size = num + 1;
	for (int i = prev_size; i < need_size; i++)
		list.append(nullptr);

	// set
	list[num] = c;

	// check the count of init() methods
	// if there are 0 methods then use auto initialization
	if (c)
	{
		int count = 0;
		for (auto i = c->functions_init.begin(); i != c->functions_init.end(); ++i)
			count += i->data.size();
		if (count == 0)
		{
			c->null_check();
			c->initialize();
			c->init_called();
		}
	}
}

void ComponentSystem::property_swapped(const NodePtr node, int from_num, int to_num)
{
	auto it = components.find(node->getID());
	if (it == components.end())
		return;

	// swap components
	ComponentBase *tmp = it->data[from_num];
	it->data[from_num] = it->data[to_num];
	it->data[to_num] = tmp;

	if (it->data[from_num]) it->data[from_num]->property_num = from_num;
	if (it->data[to_num]) it->data[to_num]->property_num = to_num;
}

void ComponentSystem::property_removed(const NodePtr node, const PropertyPtr prop, int num)
{
	auto it = components.find(node->getID());
	if (it == components.end())
		return;

	// call shutdown()
	if (it->data[num] && !it->data[num]->shutdown_called && it->data[num]->initialized)
	{
		if (!shutdowned)
		{
			// call all shutdown() methods in order
			for (auto its = it->data[num]->functions_shutdown.begin(); its != it->data[num]->functions_shutdown.end(); ++its)
				for (int ks = 0; ks < its->data.size(); ks++)
					its->data[ks]->run();
		}

		it->data[num]->node = NodePtr();
		it->data[num]->shutdown_called = 1;
	}

	// destroy component
	delete it->data[num];
	it->data[num] = nullptr;

	// destroy key in map (node id)
	// if no one property exist
	int prop_exist = 0;
	for (int k = 0; k < it->data.size(); k++)
		if (it->data[k] != nullptr)
		{
			prop_exist = 1;
			break;
		}
	if (!prop_exist)
		components.remove(it);
}

void ComponentSystem::destroy_nodes()
{
	if (!destroy_node_queue.size())
		return;

	for (int i = 0; i < destroy_node_queue.size(); i++)
	{
		if (!destroy_node_queue[i])
			continue;

		destroy_node_with_children(destroy_node_queue[i]);
	}
	destroy_node_queue.clear();
}

void ComponentSystem::destroy_node_with_children(NodePtr &node)
{
	// destroy children first
	while (node->getNumChildren())
	{
		NodePtr child = node->getChild(0);
		destroy_node_with_children(child);
	}

	// remove from editor
	if (Editor::get()->isNode(node))
		Editor::get()->releaseNode(node, 0);

	// call shutdown() to all components and release pointer
	VectorStack<ComponentBase*> all_components;
	getComponents<ComponentBase>(node, all_components);
	for (int i = 0; i < all_components.size(); i++)
	{
		// if the component was shutdowned already
		if (all_components[i]->shutdown_called)
			continue;

		// if the world is not unloaded yet
		if (!shutdowned)
		{
			// call shutdown() methods in order
			for (auto its = all_components[i]->functions_shutdown.begin(); its != all_components[i]->functions_shutdown.end(); ++its)
			for (int ks = 0; ks < its->data.size(); ks++)
				its->data[ks]->run();
		}

		all_components[i]->node = NodePtr();
		all_components[i]->shutdown_called = 1;
	}

	// destroy node
	node->grab();
	node.destroy();
}

void ComponentSystem::render_profiler()
{
	Profiler* profiler = Profiler::get();
	if (profiler->isEnabled())
	{
		float time_ms = (float)(profiler_time) / (Timer::CLOCKS_PER_SECOND / 1000);
		profiler->setValue("Components: ", "ms", time_ms, 50.0f, vec4(1, 1, 1, 1));
	}
	profiler_time = 0;
}

#ifndef __GNUC__
#pragma endregion System
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component Variables
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Variables
#endif

ComponentVariable::ComponentVariable(PropertyWrapper* component, const char *in_name, int in_type,
	const char *in_title, const char *in_tooltip, const char *in_group)
{
	holder = component;

	name = in_name;
	title = in_title;
	tooltip = in_tooltip;
	group = in_group;
	type = in_type;

	if (!name.empty())
		setParameter(
			component->property ?
			component->property->getParameterPtr(name.get()) :
			PropertyParameterPtr());

	component->variables.append(this);
}

ComponentVariable::~ComponentVariable()
{
}

void ComponentVariable::setParameter(const PropertyParameterPtr &param)
{
	parameter = param;
}

const char *ComponentVariable::getTypeName() const
{
	switch (type)
	{
		case (Property::PARAMETER_INT): return "int";
		case (Property::PARAMETER_FLOAT): return "float";
		case (Property::PARAMETER_DOUBLE): return "double";
		case (Property::PARAMETER_TOGGLE): return "toggle";
		case (Property::PARAMETER_SWITCH): return "switch";
		case (Property::PARAMETER_STRING): return "string";
		case (Property::PARAMETER_COLOR): return "color";
		case (Property::PARAMETER_VEC3): return "vec3";
		case (Property::PARAMETER_VEC4): return "vec4";
		case (Property::PARAMETER_MASK): return "mask";
		case (Property::PARAMETER_FILE): return "file";
		case (Property::PARAMETER_PROPERTY) : return "property";
		case (Property::PARAMETER_MATERIAL) : return "material";
		case (Property::PARAMETER_NODE) : return "node";
		case (Property::PARAMETER_ARRAY) : return "array";
		case (Property::PARAMETER_STRUCT) : return "struct";
		default:
			return nullptr;
	}
}

int ComponentVariable::is_type_name(const char *in_name) const
{
	String value(in_name);
	value.lower();

	if (value == "int" || 
		value == "float" ||
		value == "double" ||
		value == "toggle" ||
		value == "switch" ||
		value == "string" ||
		value == "color" ||
		value == "vec3" ||
		value == "vec4" ||
		value == "mask" ||
		value == "file" ||
		value == "property" ||
		value == "material" ||
		value == "node" ||
		value == "array" ||
		value == "struct")
		return 1;

	return 0;
}

void ComponentVariable::save(const XmlPtr &in_parameter)
{
	in_parameter->setArg("name", getName());
	in_parameter->setArg("type", getTypeName());
	if (!title.empty()) in_parameter->setArg("title", title.get());
	if (!tooltip.empty()) in_parameter->setArg("tooltip", tooltip.get());
	if (!group.empty()) in_parameter->setArg("group", group.get());
	in_parameter->setData(getValueAsString().get());
}

void ComponentVariableSwitch::save(const XmlPtr &in_parameter)
{
	ComponentVariable::save(in_parameter);
	if (!items.empty()) in_parameter->setArg("items", items.get());
}

void ComponentVariableStructBase::save(const XmlPtr &in_parameter)
{
	ComponentVariable::save(in_parameter);
	in_parameter->setArg("type", struct_type.get());
}

void ComponentVariableStructBase::setParameter(const PropertyParameterPtr &param)
{
	ComponentVariable::setParameter(param);

	// set parameters to all children inside this struct
	if (param)
		for (int i = 0; i < value_base->variables.size(); i++)
			value_base->variables[i]->setParameter(param->getChild(i));
}

#define DECLARE_VARIABLE_SETTER(TYPE_LOWERCASE, TYPE_UPPERCASE, TYPE_CAMELCASE1, TYPE_CAMELCASE2, VALUE)	\
ComponentVariable##TYPE_CAMELCASE1 &ComponentVariable##TYPE_CAMELCASE1::operator=(TYPE_LOWERCASE value_in)	\
{																											\
	VALUE = value_in;																						\
																											\
	if (!parameter || type != Property::PARAMETER_##TYPE_UPPERCASE)											\
		return *this;																						\
																											\
	parameter->setValue(value_in);																			\
	return *this;																							\
}

DECLARE_VARIABLE_SETTER(int, INT, Int, Int, value);
DECLARE_VARIABLE_SETTER(int, TOGGLE, Toggle, Toggle, value);
DECLARE_VARIABLE_SETTER(int, SWITCH, Switch, Switch, value);
DECLARE_VARIABLE_SETTER(int, MASK, Mask, Mask, value);
DECLARE_VARIABLE_SETTER(float, FLOAT, Float, Float, value);
DECLARE_VARIABLE_SETTER(double, DOUBLE, Double, Double, value);
DECLARE_VARIABLE_SETTER(const char *, STRING, String, String, value);
DECLARE_VARIABLE_SETTER(const vec3 &, VEC3, Vec3, Vec3, value);
DECLARE_VARIABLE_SETTER(const vec4 &, VEC4, Vec4, Vec4, value);
DECLARE_VARIABLE_SETTER(const vec4 &, COLOR, Color, Color, value);
DECLARE_VARIABLE_SETTER(const char *, FILE, File, File, value);
DECLARE_VARIABLE_SETTER(const PropertyPtr &, PROPERTY, Property, Property, value_guid = value_in->getGUID(); value);
DECLARE_VARIABLE_SETTER(const UGUID &, PROPERTY, Property, GUID, value = PropertyPtr(); value_guid);
DECLARE_VARIABLE_SETTER(const MaterialPtr &, MATERIAL, Material, Material, value_guid = value_in->getGUID(); value);
DECLARE_VARIABLE_SETTER(const UGUID &, MATERIAL, Material, GUID, value = MaterialPtr(); value_guid);
DECLARE_VARIABLE_SETTER(const NodePtr &, NODE, Node, Node, value_id = value_in->getID(); value);
DECLARE_VARIABLE_SETTER(int, NODE, Node, NodeID, value = NodePtr(); value_id);

#define DECLARE_VARIABLE_GETTER(TYPE_LOWERCASE, TYPE_UPPERCASE, TYPE_CAMELCASE1, TYPE_CAMELCASE2, VALUE)	\
ComponentVariable##TYPE_CAMELCASE1::operator TYPE_LOWERCASE() const											\
{																											\
	if (!parameter || type != Property::PARAMETER_##TYPE_UPPERCASE)											\
		return VALUE;																						\
																											\
	return parameter->getValue##TYPE_CAMELCASE2();															\
}

ComponentVariableString::operator const char *() const
{
	if (!parameter || type != Property::PARAMETER_STRING)
		return value;
	value = parameter->getValueString();
	return value.get();
}

DECLARE_VARIABLE_GETTER(int, INT, Int, Int, value);
DECLARE_VARIABLE_GETTER(int, TOGGLE, Toggle, Toggle, value);
DECLARE_VARIABLE_GETTER(int, SWITCH, Switch, Switch, value);
DECLARE_VARIABLE_GETTER(int, MASK, Mask, Mask, value);
DECLARE_VARIABLE_GETTER(float, FLOAT, Float, Float, value);
DECLARE_VARIABLE_GETTER(double, DOUBLE, Double, Double, value);
DECLARE_VARIABLE_GETTER(vec3, VEC3, Vec3, Vec3, value);
DECLARE_VARIABLE_GETTER(vec4, VEC4, Vec4, Vec4, value);
DECLARE_VARIABLE_GETTER(vec4, COLOR, Color, Color, value);
DECLARE_VARIABLE_GETTER(const char *, FILE, File, File, value);
DECLARE_VARIABLE_GETTER(PropertyPtr, PROPERTY, Property, Property, value);
DECLARE_VARIABLE_GETTER(UGUID, PROPERTY, Property, GUID, value_guid);
DECLARE_VARIABLE_GETTER(MaterialPtr, MATERIAL, Material, Material, value);
DECLARE_VARIABLE_GETTER(UGUID, MATERIAL, Material, GUID, value_guid);
DECLARE_VARIABLE_GETTER(NodePtr, NODE, Node, Node, value);
DECLARE_VARIABLE_GETTER(int, NODE, Node, NodeID, value_id);

#define DECLARE_VARIABLE_GETTER2(TYPE, TYPE_UPPERCASE, TYPE_CAMELCASE)					\
TYPE ComponentVariable##TYPE_CAMELCASE::get() const										\
{																						\
	if (!parameter || type != Property::PARAMETER_##TYPE_UPPERCASE)						\
		return value;																	\
																						\
	return parameter->getValue##TYPE_CAMELCASE();										\
}

const char *ComponentVariableString::get() const
{
	if (!parameter || type != Property::PARAMETER_STRING)
		return value;
	
	value = parameter->getValueString();
	return value.get();
}

const char *ComponentVariableFile::getRaw() const
{
	if (!parameter || type != Property::PARAMETER_FILE)
		return value;
	value = parameter->getValueString();
	return value.get();
}

DECLARE_VARIABLE_GETTER2(int, INT, Int);
DECLARE_VARIABLE_GETTER2(int, TOGGLE, Toggle);
DECLARE_VARIABLE_GETTER2(int, SWITCH, Switch);
DECLARE_VARIABLE_GETTER2(int, MASK, Mask);
DECLARE_VARIABLE_GETTER2(float, FLOAT, Float);
DECLARE_VARIABLE_GETTER2(double, DOUBLE, Double);
DECLARE_VARIABLE_GETTER2(vec3, VEC3, Vec3);
DECLARE_VARIABLE_GETTER2(vec4, VEC4, Vec4);
DECLARE_VARIABLE_GETTER2(vec4, COLOR, Color);
DECLARE_VARIABLE_GETTER2(const char *, FILE, File);
DECLARE_VARIABLE_GETTER2(PropertyPtr, PROPERTY, Property);
DECLARE_VARIABLE_GETTER2(MaterialPtr, MATERIAL, Material);
DECLARE_VARIABLE_GETTER2(NodePtr, NODE, Node);

#define DECLARE_VARIABLE_POINTER(TYPE, TYPE_UPPERCASE)									\
TYPE *ComponentVariable##TYPE::operator->()												\
{																						\
	if (!parameter || type != Property::PARAMETER_##TYPE_UPPERCASE)						\
		return value.get();																\
																						\
	TYPE##Ptr param_value = parameter->getValue##TYPE();								\
	if (value != param_value)															\
		value = param_value;															\
																						\
	return value.get();																	\
}

DECLARE_VARIABLE_POINTER(Property, PROPERTY);
DECLARE_VARIABLE_POINTER(Material, MATERIAL);
DECLARE_VARIABLE_POINTER(Node, NODE);

#ifndef __GNUC__
#pragma endregion Variables
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component Bases
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Base
#endif

PropertyWrapper::PropertyWrapper(const PropertyPtr &prop)
	: property(prop)
{
}

int PropertyWrapper::findStructIndex(const char *name)
{
	for (int i = 0; i < structs.size(); i++)
		if (structs[i].name == name)
			return i;

	return -1;
}

ComponentBase::ComponentBase(const NodePtr &node, int num)
	: PropertyWrapper(node ? node->getProperty(num) : PropertyPtr())
	, node(node)
	, property_num(num)
	, node_id(node ? node->getID() : 0)
	, prev_enabled(isEnabled())
	, initialized(0)
	, init_calls_count(0)
	, all_init_called(0)
	, shutdown_called(0)
	, destroy_callback(nullptr)
{
}

ComponentBase::~ComponentBase()
{
	if (destroy_callback)
		destroy_callback->run();
	delete destroy_callback;
}

int ComponentBase::is_equals(const XmlPtr &xml1, const XmlPtr &xml2)
{
	if (xml1->getNumChildren() != xml2->getNumChildren())
		return 0;

	if (xml1->getNumArgs() != xml2->getNumArgs())
		return 0;

	if (strcmp(xml1->getData(), xml2->getData()))
		return 0;

	for (int i = 0; i < xml1->getNumArgs(); i++)
		if (strcmp(xml1->getArgName(i), xml2->getArgName(i)) ||
			strcmp(xml1->getArgValue(i), xml2->getArgValue(i)))
			return 0;

	for (int i = 0; i < xml1->getNumChildren(); i++)
		if (!is_equals(xml1->getChild(i), xml2->getChild(i)))
			return 0;

	return 1;
}

void ComponentBase::save_property(const char *name)
{
	auto mkdir = [](const char *name)
	{
#ifdef _WIN32
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, (int)(sizeof(wbuf) / sizeof(wchar_t)));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		for (wchar_t *d = wbuf; *d != '\0'; d++)
		{
			if (*d == '/')
				*d = '\\';
			if (*d == '\\' && *(d + 1) == '\0')
				*d = '\0';
		}

		return (_wmkdir(wbuf) == 0);
#else
		return (::mkdir(name, 0755) == 0);
#endif
	};

	StringArray<> engine_version = String::split(Console::get()->getString("engine_version_info"), " ");

	// check parent property (create if it doesn't exist)
	StringStack<> parent_prop_name = getParentPropertyName();
	if (parent_prop_name != "node_base" &&
		!Properties::get()->findManualProperty(parent_prop_name))
	{
		mkdir(String::format("%s/ComponentSystem", Engine::get()->getDataPath()).get());
		XmlPtr parent_xml = Xml::create("property");
		parent_xml->setArg("version", engine_version[0]);
		parent_xml->setArg("name", parent_prop_name);
		parent_xml->setArg("manual", "1");
		parent_xml->setArg("parent_name", "node_base");

		StringStack<> parent_prop_path = String::format("ComponentSystem/%s.prop", parent_prop_name.get());
		parent_xml->save(parent_prop_path);
		Properties::get()->loadProperty(parent_prop_path);
		Log::message("ComponentSystem: property \"%s\" was created (%s)\n", parent_prop_name.get(), parent_prop_path.get());
	}

	// check current property (create if it doesn't exist)
	StringStack<> file_name;
	auto prop = Properties::get()->findManualProperty(name);
	if (prop)
		file_name = prop->getPath();
	else
	{
		mkdir(String::format("%s/ComponentSystem", Engine::get()->getDataPath()).get());
		file_name = String::format("ComponentSystem/%s.prop", name).get();
	}

	// create property xml
	XmlPtr xml = Xml::create("property");
	xml->setArg("version", engine_version[0]);
	xml->setArg("name", name);
	xml->setArg("manual", "1");
	xml->setArg("parent_name", parent_prop_name);

	// save structs
	Vector<String> saved_structs;
	std::function<void(PropertyWrapper *)> save_structs = [&, this](PropertyWrapper *c)
	{
		// go deeper
		for (int i = 0; i < c->variables.size(); i++)
			if (c->variables[i]->getType() == Property::PARAMETER_STRUCT)
			{
				ComponentVariableStructBase *cvs = dynamic_cast<ComponentVariableStructBase*>(c->variables[i]);
				save_structs(cvs->getBase());
			}

		// save structs from far to near
		for (int i = 0; i < c->structs.size(); i++)
		{
			String &name = c->structs[i].name;

			// add new struct and save it to xml
			if (saved_structs.findIndex(name) == -1)
			{
				XmlPtr x = xml->addChild("struct");
				x->setArg("name", name.get());
				ComponentStruct *data = c->structs[i].instance;
				for (int k = 0; k < data->variables.size(); k++)
					if (data->variables[k]->getName()[0] != '\0')
						data->variables[k]->save(x->addChild("parameter"));
				saved_structs.append(name);
			}
		}
	};
	save_structs(this);
	
	// save parameters
	for (int i = 0; i < variables.size(); i++)
		if (variables[i]->getName()[0] != '\0')
			variables[i]->save(xml->addChild("parameter"));
	
	// check for changes with previous property
	int need_to_save = 1;
	if (prop)
	{
		XmlPtr xml_prev = Xml::create();
		if (xml_prev->load(file_name.get()) && is_equals(xml_prev, xml))
			need_to_save = 0;
	}

	// save to file
	if (need_to_save)
	{
		xml->save(file_name.get());
		Properties::get()->loadProperty(file_name.get());
		Log::message("ComponentSystem: property \"%s\" was %s (%s)\n", name, (prop ? "modified" : "created"), file_name.get());
	}
}

void ComponentBase::destroyNode(const NodePtr &in_node)
{
	ComponentSystem::get()->destroyNode(in_node);
}

void ComponentBase::null_check()
{
	if (ComponentSystem::get()->getWarningLevel() < ComponentSystem::WARNING_LEVEL::HIGH)
		return;

	for (int i = 0; i < variables.size(); i++)
		if (variables[i]->nullCheck())
			Log::warning("ComponentBase::null_check(): parameter \"%s\" of the property \"%s\" is null\n",
			variables[i]->getName(), property->getName());
}

void ComponentBase::initialize()
{
	// release ownership
	// (destroy of this component shouldn't destroy the node)
	if (node)
		node->release();

	initialized = 1;
}

void ComponentBase::init_called()
{
	init_calls_count++;

	int count = 0;
	for (auto i = functions_init.begin(); i != functions_init.end(); ++i)
		count += i->data.size();
	
	if (init_calls_count >= count)
		all_init_called = 1;
}

#ifndef __GNUC__
#pragma endregion Base
#endif
