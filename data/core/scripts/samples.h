#include <core/scripts/primitives.h>
#include <core/scripts/utils.h>
#include <core/unigine.h>

namespace Unigine::Samples
{
Node addToEditor(Node node)
{
	engine.editor.addNode(node, 1);
	return node_remove(node);
}

void removeFromEditor(Node node)
{
	engine.editor.removeNode(node);
}

string fullPath(string path) {
	return engine.filesystem.resolvePartialVirtualPath(path);
}

//-----------------------------------------------------------------------------
// DirInterface
//-----------------------------------------------------------------------------

class DirInterface
{
	string path;
	Gui gui;
	WidgetVBox vbox;
	
	void init()
	{
		string names[0];
		Dir directory = new Dir();
		if(directory.open(engine.filesystem.getAbsolutePath(path)))
		{
			forloop(int i = 0; directory.getNumDirs())
			{
				Dir samples = new Dir();
				if(!samples.open(directory.getDirName(i)))
				{
					delete samples;
					continue;
				}

				string files[0];
				forloop(int j = 0; samples.getNumFiles())
					files.append(samples.getFileName(j));

				foreach(string file; files)
				{
					if(strrstr(file,".world") == -1) continue;
					if(strrstr(file,".meta") != -1) continue;
					file = relname(engine.filesystem.getAbsolutePath(path) + "/",file);
					names.append(replace(file,".world",""));
					break;
				}

				samples.close();
				delete samples;
			}
			directory.close();
		}

		if(names.size() == 0)
		{
			string files[0];
			engine.filesystem.getVirtualFiles(files);

			foreach(string file; files)
			{
				if(strstr(file,path) != 0) continue;

				string name = substr(file,strlen(path));
				int pos = strchr(name,'/');
				if(pos == -1) continue;

				name = substr(name,0,pos);
				if(names.find(name) != -1) continue;

				names.append(name);
			}

			forloop(int i = 0; names.size())
			{
				string name = path + names[i] + "/";
				foreach(string file; files)
				{
					if(strstr(file,name) != 0) continue;
					if(strrstr(file,".world") == -1) continue;
					if(strrstr(file,".meta") != -1) continue;

					names[i] = replace(replace(file,path,""),".world","");
					break;
				}
			}
		}

		delete directory;
		names.sort();

		gui = engine.getGui();

		WidgetGridBox gridbox = new WidgetGridBox(gui,3);
		gridbox.setPadding(0,0,4,0);

		WidgetGroupBox groupbox = new WidgetGroupBox(gui);
		groupbox.setPadding(4,4,4,4);
		groupbox.setBackground(1);
		groupbox.addChild(new WidgetLabel(gui,"Select scene"));
		groupbox.addChild(gridbox,GUI_ALIGN_EXPAND);

		vbox = new WidgetVBox(gui);
		vbox.addChild(groupbox,GUI_ALIGN_EXPAND);

		foreach(string name; names)
		{
			WidgetButton button = new WidgetButton(gui,name);
			button.setFontSize(13);
			button.setCallback(GUI_CLICKED,"Unigine::Samples::DirInterface::clicked","world_load " + path + "/" + name);
			gridbox.addChild(button,GUI_ALIGN_EXPAND);
		}
		
		gridbox.arrange();
		gui.addChild(vbox,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
	}

	void show()
	{
		gui.addChild(vbox,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
	}

	void hide()
	{
		gui.removeChild(vbox);
	}

	void clicked(string command)
	{
		engine.console.run(command);
	}

	DirInterface(string p)
	{
		path = p;
		init();
	}

	void __restore__()
	{
		init();
	}
};

DirInterface dir_interface;

void hideDirInterface()
{
	dir_interface.hide();
}

void showDirInterface()
{
	dir_interface.show();
}

void createDirInterface(string world_path)
{
	string path = engine.filesystem.resolvePartialVirtualPath(world_path);
	dir_interface = new DirInterface(pathname(path));
}

//-----------------------------------------------------------------------------
// Interface
//-----------------------------------------------------------------------------

class Interface
{
	string path;
	string default_world;
	Gui gui;
	WidgetHBox hbox;

	void init()
	{
		string names[0];

		Dir directory = new Dir();
		if (directory.open(engine.filesystem.getAbsolutePath(path)))
		{
			forloop (int i = 0; directory.getNumFiles())
			{
				string name = basename(directory.getFileName(i));
				if (extension(name) == "world")
					names.append(extension(name, NULL));
			}
			directory.close();
		}

		delete directory;

		if (names.size() == 0)
		{
			string files[0];
			engine.filesystem.getVirtualFiles(files);
			foreach (string file; files)
			{
				if (strstr(file, path) == 0 && extension(file) == "world")
					names.append(extension(basename(file), NULL));
			}
		}

		names.sort();

		gui = engine.getGui();
		hbox = new WidgetHBox(gui);
		WidgetGroupBox groupbox = new WidgetGroupBox(gui);
		groupbox.setPadding(2, 2, 2, 2);
		groupbox.setBackground(1);
		hbox.addChild(groupbox, GUI_ALIGN_EXPAND);
		groupbox.addChild(new WidgetLabel(gui, "Select scene"));
		WidgetGridBox gridbox = new WidgetGridBox(gui, 3);
		gridbox.setPadding(0, 0, 2, 0);
		groupbox.addChild(gridbox, GUI_ALIGN_EXPAND);

		WidgetButton button = new WidgetButton(gui, "../");
		button.setFontSize(11);
		button.setCallback(GUI_CLICKED, "Unigine::Samples::Interface::clicked", "world_load " + default_world);
		gridbox.addChild(button, GUI_ALIGN_EXPAND);

		foreach (string name; names)
		{
			WidgetButton button = new WidgetButton(gui, name);
			button.setFontSize(11);
			button.setCallback(GUI_CLICKED, "Unigine::Samples::Interface::clicked", "world_load " + path + name);
			gridbox.addChild(button, GUI_ALIGN_EXPAND);
		}

		gridbox.arrange();
		gui.addChild(hbox, GUI_ALIGN_RIGHT | GUI_ALIGN_BOTTOM);
	}

	void show()
	{
		gui.addChild(hbox, GUI_ALIGN_RIGHT | GUI_ALIGN_BOTTOM);
	}

	void hide()
	{
		gui.removeChild(hbox);
	}

	void clicked(string command)
	{
		engine.console.run(command);
	}

	Interface(string p, string default_w = "samples/samples.world")
	{
		path = p;
		default_world = default_w;
		init();
	}

	void __restore__()
	{
		init();
	}
};

Interface interface;

void hideInterface()
{
	interface.hide();
}

void showInterface()
{
	interface.show();
}

void createInterface(string world_path, string default_world = "samples/samples.world")
{
	string path = engine.filesystem.resolvePartialVirtualPath(world_path);
	interface = new Interface(pathname(path), default_world);
}

//-----------------------------------------------------------------------------
// Description
//-----------------------------------------------------------------------------

class Description
{
	string description;

	void init()
	{
		Gui gui = engine.getGui();
		WidgetLabel label = new WidgetLabel(gui, description);
		label.setFontSize(engine.app.getHeight() / 32);
		label.setFontOutline(1);
		label.arrange();
		label.setPosition((engine.app.getWidth() - label.getWidth()) / 2, label.getFontSize() * 2);
		gui.addChild(label, GUI_ALIGN_OVERLAP);
	}

	Description(string str)
	{
		description = str;
		init();
	}

	void __restore__()
	{
		init();
	}
};

Description description;

void setDescription(string text)
{
	description = new Description(text);
}

//-----------------------------------------------------------------------------
// Info Widget
//-----------------------------------------------------------------------------

class Info
{
	string info;
	WidgetLabel label;
	WidgetSprite sprite;

	Info()
	{
		init();
	}
	Info(string s)
	{
		init();
		set(s);
	}

	void __restore__()
	{
		init();
	}

	void init()
	{
		Gui gui = engine.getGui();
		label = new WidgetLabel(gui);
		label.setFontOutline(1);
		label.setFont("core/gui/console.ttf");
		sprite = new WidgetSprite(gui, "core/gui/gui_white.png");
		sprite.setColor(vec4(0.0f, 0.0f, 0.0f, 0.75f));
		set(info);
	}

	void set(string s)
	{
		info = s;
		label.setText(info);
		label.arrange();
		label.setPosition((engine.app.getWidth() - label.getWidth()) / 2, (engine.app.getHeight() - label.getHeight()) / 3);
		sprite.setPosition(label.getPositionX() - 8, label.getPositionY() - 8);
		sprite.setWidth(label.getWidth() + 16);
		sprite.setHeight(label.getHeight() + 16);
		engine.gui.addChild(sprite, GUI_ALIGN_OVERLAP);
		engine.gui.addChild(label, GUI_ALIGN_OVERLAP);
	}
};

//-----------------------------------------------------------------------------
// Player
//-----------------------------------------------------------------------------

PlayerSpectator createDefaultPlayer(Vec3 pos, float min_speed = 20.0f, float max_speed = 40.0f)
{
	PlayerSpectator player = new PlayerSpectator();
	player.setPosition(pos);
	player.setDirection(vec3(-1.0f, 0.0f, -0.5f));
	player.setMinVelocity(min_speed);
	player.setMaxVelocity(max_speed);
	engine.game.setPlayer(player);
	return player;
}

PlayerSpectator createDefaultPlayer(Vec3 pos, quat direction, float min_speed = 50.0f, float max_speed = 100.0f)
{
	PlayerSpectator player = new PlayerSpectator();
	player.setPosition(pos);
	player.setWorldRotation(direction);
	player.setMinVelocity(min_speed);
	player.setMaxVelocity(max_speed);
	engine.game.setPlayer(player);
	return player;
}

//-----------------------------------------------------------------------------
// Material
//-----------------------------------------------------------------------------

string getMaterial(int index, string material_names[]) {
	return material_names[abs(index) % material_names.size()];
}

//-----------------------------------------------------------------------------
// Ground
//-----------------------------------------------------------------------------

ObjectMeshDynamic createDefaultPlane()
{
	ObjectMeshDynamic plane = addToEditor(Unigine::createPlane(1000.0f, 1000.0f, 1000.0f));
	plane.setWorldTransform(Mat4_identity);
	plane.setMaterial("ground_grid", "*");
	return plane;
}

ObjectMeshDynamic createPlaneWithBody()
{
	ObjectMeshDynamic plane = addToEditor(Unigine::createPlane(1000.0f, 1000.0f, 1000.0f));
	plane.setWorldTransform(Mat4_identity);
	plane.setMaterial("ground_grid", "*");
	BodyDummy body = class_remove(new BodyDummy(plane));
	ShapeBox shape = class_remove(new ShapeBox(vec3(1000.0f, 1000.0f, 10.0f)));
	body.addShape(shape, translate(0.0f, 0.0f, -5.0f));
	return plane;
}

//-----------------------------------------------------------------------------
// Primitives
//-----------------------------------------------------------------------------

/* box
	 */
ObjectMeshDynamic boxes[];

Object createObjectBox(vec3 size, string material, Mat4 transform)
{
	string hash = string(size);
	if (boxes.check(hash) == 0)
	{
		Object box = Unigine::createBox(size);
		boxes.append(hash, box);
		box.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(boxes[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyBox(vec3 size, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = string(size);
	if (boxes.check(hash) == 0)
	{
		ObjectMeshDynamic obj = Unigine::createBox(size);
		obj.setEnabled(0);
		boxes.append(hash, obj);
	}
	Object mesh = addToEditor(node_append(boxes[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeBox shape = class_remove(new ShapeBox(body, size));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* sphere
	 */
ObjectMeshDynamic spheres[];

Object createObjectSphere(float radius, string material, Mat4 transform)
{
	string hash = string(radius);
	if (spheres.check(hash) == 0)
	{
		Object sphere = Unigine::createSphere(radius, 6, 12);
		spheres.append(hash, sphere);
		sphere.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(spheres[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodySphere(float radius, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = string(radius);
	if (spheres.check(hash) == 0)
	{
		Object sphere = Unigine::createSphere(radius, 6, 12);
		spheres.append(hash, sphere);
		sphere.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(spheres[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeSphere shape = class_remove(new ShapeSphere(body, radius));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* capsule
	 */
ObjectMeshDynamic capsules[];

Object createObjectCapsule(float radius, float height, string material, Mat4 transform)
{
	string hash = format("%g %g", radius, height);
	if (capsules.check(hash) == 0)
	{
		Object capsule = Unigine::createCapsule(radius, height, 7, 12);
		capsules.append(hash, capsule);
		capsule.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(capsules[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyCapsule(float radius, float height, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = format("%g %g", radius, height);
	if (capsules.check(hash) == 0)
	{
		Object capsule = Unigine::createCapsule(radius, height, 7, 12);
		capsules.append(hash, capsule);
		capsule.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(capsules[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeCapsule shape = class_remove(new ShapeCapsule(body, radius, height));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* cylinder
	 */
ObjectMeshDynamic cylinders[];

Object createObjectCylinder(float radius, float height, string material, Mat4 transform)
{
	string hash = format("%g %g", radius, height);
	if (cylinders.check(hash) == 0)
	{
		Object cylinder = Unigine::createCylinder(radius, height, 1, 12);
		cylinders.append(hash, cylinder);
		cylinder.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(cylinders[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyCylinder(float radius, float height, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = format("%g %g", radius, height);
	if (cylinders.check(hash) == 0)
	{
		Object cylinder = Unigine::createCylinder(radius, height, 1, 12);
		cylinders.append(hash, cylinder);
		cylinder.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(cylinders[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeCylinder shape = class_remove(new ShapeCylinder(body, radius, height));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* prism
	 */
ObjectMeshDynamic prisms[];

Object createObjectPrism(float size_0, float size_1, float height, int sides, string material, Mat4 transform)
{
	string hash = format("%f %f %f %d", size_0, size_1, height, sides);
	if (prisms.check(hash) == 0)
	{
		Object prism = Unigine::createPrism(size_0, size_1, height, sides);
		prisms.append(hash, prism);
		prism.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(prisms[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyPrism(float size_0, float size_1, float height, int sides, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = format("%f %f %f %d", size_0, size_1, height, sides);
	if (prisms.check(hash) == 0)
	{
		Object prism = Unigine::createPrism(size_0, size_1, height, sides);
		prisms.append(hash, prism);
		prism.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(prisms[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeConvex shape = class_remove(new ShapeConvex(body, mesh, -1));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* icosahedron
	 */
ObjectMeshDynamic icosahedrons[];

Object createObjectIcosahedron(float radius, string material, Mat4 transform)
{
	string hash = string(radius);
	if (icosahedrons.check(hash) == 0)
	{
		Object icosahedron = Unigine::createIcosahedron(radius);
		icosahedrons.append(hash, icosahedron);
		icosahedron.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(icosahedrons[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyIcosahedron(float radius, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = string(radius);
	if (icosahedrons.check(hash) == 0)
	{
		Object icosahedron = Unigine::createIcosahedron(radius);
		icosahedrons.append(hash, icosahedron);
		icosahedron.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(icosahedrons[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeConvex shape = class_remove(new ShapeConvex(body, mesh, -1));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* dodecahedron
	 */
ObjectMeshDynamic dodecahedrons[];

Object createObjectDodecahedron(float radius, string material, Mat4 transform)
{
	string hash = string(radius);
	if (dodecahedrons.check(hash) == 0)
	{
		Object dodecahedron = Unigine::createDodecahedron(radius);
		dodecahedrons.append(hash, dodecahedron);
		dodecahedron.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(dodecahedrons[hash].clone()));
	mesh.setWorldTransform(transform);
	mesh.setMaterial(material, "*");
	mesh.setEnabled(1);
	return mesh;
}

Body createBodyDodecahedron(float radius, float density, float friction, float restitution, string material, Mat4 transform)
{
	string hash = string(radius);
	if (dodecahedrons.check(hash) == 0)
	{
		Object dodecahedron = Unigine::createDodecahedron(radius);
		dodecahedrons.append(hash, dodecahedron);
		dodecahedron.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(dodecahedrons[hash].clone()));
	mesh.setEnabled(1);
	Body body = class_remove((density > 0.0f) ? new BodyRigid(mesh) : new BodyDummy(mesh));
	ShapeConvex shape = class_remove(new ShapeConvex(body, mesh, -1));
	shape.setDensity(density);
	shape.setFriction(friction);
	shape.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* rope
	 */
Body createBodyRope(float radius, float height, float step, float mass, float friction, float restitution, string material, Mat4 transform)
{
	ObjectMeshDynamic mesh = addToEditor(Unigine::createCylinder(radius, height, int(height / step), 7));
	BodyRope body = class_remove(new BodyRope(mesh));
	body.setMass(mass);
	body.setRadius(radius);
	body.setFriction(friction);
	body.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* cloth
	 */
Body createBodyCloth(float width, float height, float step, float mass, float friction, float restitution, string material, Mat4 transform)
{
	ObjectMeshDynamic mesh = addToEditor(Unigine::createPlane(width, height, step));
	BodyCloth body = class_remove(new BodyCloth(mesh));
	body.setMass(mass);
	body.setFriction(friction);
	body.setRestitution(restitution);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* fracture
	 */
Body createBodyFracture(vec3 size, float density, float friction, float restitution, string material, string crack, Mat4 transform)
{
	string hash = string(size);
	if (boxes.check(hash) == 0)
	{
		Object box = Unigine::createBox(size);
		boxes.append(hash, box);
		box.setEnabled(0);
	}
	Object mesh = addToEditor(node_append(boxes[hash].clone()));
	mesh.setEnabled(1);
	BodyFracture body = class_remove(new BodyFracture(mesh));
	body.setDensity(density);
	body.setFriction(friction);
	body.setRestitution(restitution);
	body.setMaterial(crack);
	mesh.setMaterial(material, "*");
	mesh.setTransform(transform);
	return body;
}

/* Homuncle (materials - 5 different materials)
	 */
Body createBodyHomuncle(int frozen, vec3 velocity, string materials[], Mat4 transform)
{
	assert(materials.size() >= 5 && "createBodyHomuncle :: at least 5 materials expected");

	NodeDummy homuncle = addToEditor(new NodeDummy());
	homuncle.setName("homuncle");

	BodyRigid create_body(string name, float radius, float height, float mass, string material, Mat4 t)
	{
		BodyRigid body = createBodyCapsule(radius, height, 1.0f, 0.5f, 0.0f, material, transform * t);
		homuncle.addWorldChild(body.getObject());
		Object(body.getObject()).setName(name);
		Shape(body.getShape(0)).setMass(mass);
		body.setName("homuncle");
		body.setLinearVelocity(velocity);
		body.setLinearDamping(0.1f);
		body.setAngularDamping(0.1f);
		body.setFrozen(frozen);
		return body;
	}

	JointBall create_joint(Body b0, Body b1, Vec3 anchor, vec3 axis, float angle, float from, float to)
	{
		JointBall joint = class_remove(new JointBall(b0, b1));
		joint.setWorldAnchor(transform * anchor);
		joint.setWorldAxis(rotation(transform) * axis);
		joint.setAngularLimitAngle(angle);
		joint.setAngularLimitFrom(from);
		joint.setAngularLimitTo(to);
		joint.setAngularDamping(16.0f);
		joint.setLinearRestitution(0.2f);
		joint.setAngularRestitution(0.1f);
		joint.setLinearSoftness(0.0f);
		joint.setAngularSoftness(0.0f);
		joint.setNumIterations(1);
		return joint;
	}

	JointHinge create_joint(Body b0, Body b1, Vec3 anchor, vec3 axis, float from, float to)
	{
		JointHinge joint = class_remove(new JointHinge(b0, b1));
		joint.setWorldAnchor(transform * anchor);
		joint.setWorldAxis(rotation(transform) * axis);
		joint.setAngularLimitFrom(from);
		joint.setAngularLimitTo(to);
		joint.setAngularDamping(16.0f);
		joint.setLinearRestitution(0.2f);
		joint.setAngularRestitution(0.1f);
		joint.setLinearSoftness(0.0f);
		joint.setAngularSoftness(0.0f);
		joint.setNumIterations(1);
		return joint;
	}

	// torso
	Body neck = create_body("neck", 0.45f, 0.4f, 2.0f, materials[0], translate(0.0f, 0.0f, 3.2f));
	Body spine = create_body("spine", 0.35f, 0.4f, 2.0f, materials[0], translate(0.0f, 0.0f, 2.5f));

	create_joint(neck, spine, Vec3(0.0f, 0.0f, 2.7f), vec3(0.0f, 0.0f, 1.0f), 150.0f, -10.0f, 10.0f);

	// head
	Body head = create_body("head", 0.3f, 0.15f, 0.5f, materials[1], translate(0.0f, 0.0f, 4.25f));

	create_joint(neck, head, Vec3(0.0f, 0.0f, 4.0f), vec3(0.0f, 0.0f, 1.0f), 140.0f, -25.0f, 25.0f);

	// thighs
	Body left_thigh = create_body("left_thigh", 0.24f, 0.8f, 2.0f, materials[2], translate(0.0f, 0.3f, 1.7f));
	Body right_thigh = create_body("right_thigh", 0.24f, 0.8f, 2.0f, materials[2], translate(0.0f, -0.3f, 1.7f));

	create_joint(spine, left_thigh, Vec3(0.0f, 0.3f, 2.2f), vec3(0.0f, 0.2f, -1.0f), 120.0f, -25.0f, 25.0f);
	create_joint(spine, right_thigh, Vec3(0.0f, -0.3f, 2.2f), vec3(0.0f, -0.2f, -1.0f), 120.0f, -25.0f, 25.0f);

	// calfs
	Body left_calf = create_body("left_calf", 0.2f, 0.7f, 1.0f, materials[3], translate(0, 0.3f, 0.7f));
	Body right_calf = create_body("right_calf", 0.2f, 0.7f, 1.0f, materials[3], translate(0, -0.3f, 0.7f));

	create_joint(left_thigh, left_calf, Vec3(0.0f, 0.3f, 1.1f), vec3(0.0f, 1.0f, 0.0f), -110.0f, 0.0f);
	create_joint(right_thigh, right_calf, Vec3(0.0f, -0.3f, 1.1f), vec3(0.0f, 1.0f, 0.0f), -110.0f, 0.0f);

	// foots
	Body left_foot = create_body("left_foot", 0.2f, 0.3f, 1.0f, materials[4], translate(0.2f, 0.3f, 0.1f) * rotateY(-90.0f));
	Body right_foot = create_body("right_foot", 0.2f, 0.3f, 1.0f, materials[4], translate(0.2f, -0.3f, 0.1f) * rotateY(-90.0f));

	create_joint(left_calf, left_foot, Vec3(0.0f, 0.3f, 0.1f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 30.0f);
	create_joint(right_calf, right_foot, Vec3(0.0f, -0.3f, 0.1f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 30.0f);

	// upperarms
	Body left_upperarm = create_body("left_upperarm", 0.2f, 0.7f, 2.0f, materials[2], translate(0.0f, 0.8f, 3.5f) * rotateX(90.0f));
	Body right_upperarm = create_body("right_upperarm", 0.2f, 0.7f, 2.0f, materials[2], translate(0.0f, -0.8f, 3.5f) * rotateX(90.0f));

	create_joint(neck, left_upperarm, Vec3(0.0f, 0.5f, 3.5f), vec3(0.0f, 1.0f, -0.5f), 140.0f, -45.0f, 45.0f);
	create_joint(neck, right_upperarm, Vec3(0.0f, -0.5f, 3.5f), vec3(0.0f, -1.0f, -0.5f), 140.0f, -45.0f, 45.0f);

	// forearms
	Body left_forearm = create_body("left_forearm", 0.17f, 0.6f, 1.0f, materials[3], translate(0.0f, 1.6f, 3.5f) * rotateX(90.0f));
	Body right_forearm = create_body("right_forearm", 0.17f, 0.6f, 1.0f, materials[3], translate(0.0f, -1.6f, 3.5f) * rotateX(90.0f));

	create_joint(left_upperarm, left_forearm, Vec3(0.0f, 1.3f, 3.5f), vec3(0.0f, 0.0f, 1.0f), 0.0f, 120.0f);
	create_joint(right_upperarm, right_forearm, Vec3(0.0f, -1.3f, 3.5f), vec3(0.0f, 0.0f, -1.0f), 0.0f, 120.0f);

	// hands
	Body left_hand = create_body("left_hand", 0.15f, 0.2f, 1.0f, materials[4], translate(0.0f, 2.1f, 3.5f) * rotateX(90.0f));
	Body right_hand = create_body("right_hand", 0.15f, 0.2f, 1.0f, materials[4], translate(0.0f, -2.1f, 3.5f) * rotateX(90.0f));

	create_joint(left_forearm, left_hand, Vec3(0.0f, 2.0f, 3.5f), vec3(0.0f, 1.0f, 0.0f), 120.0f, -25.0f, 25.0f);
	create_joint(right_forearm, right_hand, Vec3(0.0f, -2.0f, 3.5f), vec3(0.0f, 1.0f, 0.0f), 120.0f, -25.0f, 25.0f);

	return homuncle;
}

//-----------------------------------------------------------------------------
// SpringalLock
//-----------------------------------------------------------------------------
class SpringalLock
{
	Joint joint;
	float time;

	SpringalLock(Joint j, float t)
	{
		joint = j;
		time = t;
	}
};

SpringalLock sprinal_locks[0];

void unlockSpringals()
{
	float time = engine.physics.getFrame() * engine.physics.getIFps();

	foreach (SpringalLock lock; sprinal_locks)
	{
		if (lock.time < time)
			lock.joint.setEnabled(0);
	}
}

/* (materials - 5 different materials)
	 */
void createSpringal(float time, string materials[], Mat4 transform)
{
	assert(materials.size() >= 5 && "createSpringal :: at least 5 materials expected");

	JointSuspension create_wheel(Body b0, Body b1)
	{
		JointSuspension j = class_remove(new JointSuspension(b0, b1));
		j.setWorldAxis0(rotation(transform) * vec3(0.0f, 0.0f, 1.0f));
		j.setWorldAxis1(rotation(transform) * vec3(0.0f, 1.0f, 0.0f));
		j.setLinearSpring(60.0f);
		j.setLinearDamping(4.0f);
		j.setLinearLimitFrom(-1.0f);
		j.setLinearLimitTo(0.0f);
		j.setAngularVelocity(4.0f);
		j.setAngularTorque(8000.0f);
		j.setLinearRestitution(0.2f);
		j.setAngularRestitution(0.2f);
		j.setLinearSoftness(0.0f);
		j.setAngularSoftness(0.0f);
		j.setNumIterations(4);
		return j;
	}

	JointFixed create_fixed(Body b0, Body b1)
	{
		JointFixed j = class_remove(new JointFixed(b0, b1));
		j.setLinearRestitution(0.2f);
		j.setAngularRestitution(0.2f);
		j.setLinearSoftness(0.0f);
		j.setAngularSoftness(0.0f);
		j.setNumIterations(8);
		return j;
	}

	JointHinge create_hinge(Body b0, Body b1, Vec3 anchor, vec3 axis)
	{
		JointHinge j = class_remove(new JointHinge(b0, b1));
		j.setWorldAnchor(transform * anchor);
		j.setWorldAxis(rotation(transform) * axis);
		j.setLinearRestitution(0.2f);
		j.setAngularRestitution(0.2f);
		j.setLinearSoftness(0.0f);
		j.setAngularSoftness(0.0f);
		j.setNumIterations(8);
		j.setCollision(1);
		return j;
	}

	Body body = createBodyBox(vec3(12.0f, 6.0f, 1.0f), 100.0f, 0.5f, 0.5f, materials[0], transform * translate(0.0f, 0.0f, 2.0f));

	Body wheel_0 = createBodySphere(1.0f, 250.0f, 1.0f, 0.5f, materials[1], transform * translate(5.0f, 3.25f, 1.0f));
	Body wheel_1 = createBodySphere(1.0f, 250.0f, 1.0f, 0.5f, materials[1], transform * translate(5.0f, -3.25f, 1.0f));
	Body wheel_2 = createBodySphere(1.0f, 250.0f, 1.0f, 0.5f, materials[1], transform * translate(-5.0f, 3.25f, 1.0f));
	Body wheel_3 = createBodySphere(1.0f, 250.0f, 1.0f, 0.5f, materials[1], transform * translate(-5.0f, -3.25f, 1.0f));

	create_wheel(body, wheel_0);
	create_wheel(body, wheel_1);
	create_wheel(body, wheel_2);
	create_wheel(body, wheel_3);

	Body b00 = createBodyBox(vec3(0.8f, 0.8f, 5.0f), 1.0f, 0.5f, 0.5f, materials[0], transform * translate(-2.0f, 2.6f, 5.0f));
	Body b01 = createBodyBox(vec3(0.8f, 0.8f, 5.0f), 1.0f, 0.5f, 0.5f, materials[0], transform * translate(-2.0f, -2.6f, 5.0f));

	create_fixed(body, b00);
	create_fixed(body, b01);

	Body b10 = createBodyBox(vec3(18.0f, 2.0f, 0.5f), 30.0f, 0.5f, 0.5f, materials[2], transform * translate(4.0f, 0.0f, 7.0f));
	Body b11 = createBodyBox(vec3(3.0f, 4.0f, 4.0f), 400.0f, 0.5f, 0.5f, materials[3], transform * translate(-4.0f, 0.0f, 7.0f));

	create_fixed(b10, b11);

	create_hinge(body, b10, Vec3(-2.0f, 0.0f, 7.0f), vec3(0.0f, 1.0f, 0.0f));

	JointFixed lock = create_fixed(body, b11);

	sprinal_locks.append(new SpringalLock(lock, time));

	createBodyHomuncle(0, vec3(0.0f), materials, transform * translate(6.6f, 0.0f, 7.5f) * rotateY(90.0f) * rotateZ(180.0f));
}

//-----------------------------------------------------------------------------
// Tracks
//-----------------------------------------------------------------------------
void createTracks(vec3 size, float density, float friction, float restitution, float step, int num, string material, Mat4 transform)
{
	BodyRigid create_body(Vec3 position)
	{
		BodyRigid body = createBodyBox(size, density, friction, restitution, material, translate(position));
		body.setFrozen(1);
		return body;
	}

	JointHinge create_joint(Body b0, Body b1)
	{
		JointHinge joint = class_remove(new JointHinge(b0, b1));
		joint.setAnchor0(Vec3(0.0f, step * 0.5f, 0.0f));
		joint.setAnchor1(Vec3(0.0f, -step * 0.5f, 0.0f));
		joint.setAngularDamping(16.0f);
		joint.setLinearRestitution(0.4f);
		joint.setAngularRestitution(0.1f);
		joint.setLinearSoftness(0.1f);
		joint.setAngularSoftness(0.1f);
		joint.setNumIterations(4);
		return joint;
	}

	Body bodies[0];

	forloop(int i = 0; num * 4 + 6)
	{
		bodies.append(create_body(Vec3(0.0f, step * i, 0.0f)));
	}

	forloop(int i = 0; bodies.size())
	{
		create_joint(bodies[i], bodies[(i + 1) % bodies.size()]);
	}

	float radius = step * cos(30.0f * DEG2RAD);
	float offset = step * (sin(30.0f * DEG2RAD) * 0.5f + 0.5f + num);

	forloop(int i = -num; num + 1)
	{
		bodies[num * 1 + 2 + i].setTransform(transform * translate(0.0f, step * i, radius));
		bodies[num * 3 + 5 + i].setTransform(transform * translate(0.0f, -step * i, -radius) * rotateX(180.0f));
	}

	bodies[0].setTransform(transform * translate(0.0f, -offset, -radius * 0.5f) * rotateX(120.0f));
	bodies[1].setTransform(transform * translate(0.0f, -offset, radius * 0.5f) * rotateX(60.0f));

	bodies[num * 2 + 3].setTransform(transform * translate(0.0f, offset, radius * 0.5f) * rotateX(300.0f));
	bodies[num * 2 + 4].setTransform(transform * translate(0.0f, offset, -radius * 0.5f) * rotateX(240.0f));

	forloop(int i = 0; bodies.size())
	{
		Node(bodies[i].getObject()).setTransform(bodies[i].getTransform());
	}
}


/* update physics
	 */
int updatePhysics()
{
// drop sphere
#ifdef HAS_APP_TABLET
	if (engine.app.clearMouseButtonState(APP_BUTTON_DCLICK))
	{
		Player player = engine.game.getPlayer();
		BodyRigid b = create_sphere(0.5f, 20, 0.5f, 1, 0, inverse(player.getIWorldTransform()) * translate(0, 0, -1.5f));
		b.setLinearVelocity(player.getDirection() * 100.0f);
		b.setName("sphere");
	}
#else
	int pressed;
	if (engine.editor.isLoaded() == 0 && engine.controls.isMouseEnabled())
	{
		if (pressed == 0 && engine.app.getMouseButtonState(APP_BUTTON_LEFT | APP_BUTTON_DCLICK))
			pressed = 1;
		else if (pressed == 1 && engine.app.getMouseButtonState(APP_BUTTON_LEFT | APP_BUTTON_DCLICK) == 0)
			pressed = 2;
		else if (pressed == 2 && engine.app.getMouseButtonState(APP_BUTTON_LEFT | APP_BUTTON_DCLICK))
		{
			Player player = engine.game.getPlayer();
			BodyRigid b = createBodySphere(0.5f, 20, 0.5f, 1, "mesh_bullet", inverse(player.getIWorldTransform()) * translate(0, 0, -1.5f));
			b.setLinearVelocity(player.getDirection() * 100.0f);
			b.setName("sphere");
			pressed = 1;
		}
	} else
		pressed = 0;
#endif

	// camera
	int mouse_x;
	int mouse_y;
	if (engine.editor.isLoaded() == 0 && engine.controls.isMouseEnabled() == 0)
	{
		if (engine.app.getMouseButtonState(APP_BUTTON_MIDDLE))
		{
			float dx = (engine.app.getMouseX() - mouse_x) * engine.controls.getMouseSensitivity() * 0.2f;
			float dy = (engine.app.getMouseY() - mouse_y) * engine.controls.getMouseSensitivity() * 0.2f;
			engine.controls.setMouseDX(dx);
			engine.controls.setMouseDY(dy);
		}
		mouse_x = engine.app.getMouseX();
		mouse_y = engine.app.getMouseY();
	}

	// spring
	float radius;
	vec3 position;
	Object object;
	if (engine.editor.isLoaded() == 0 && engine.controls.isMouseEnabled() == 0)
	{
		if (engine.app.getMouseButtonState(APP_BUTTON_RIGHT))
		{
			vec3 p0, p1;
			Unigine::getPlayerMouseDirection(p0, p1);
			if (object == NULL)
			{
				PhysicsIntersection intersection = new PhysicsIntersection();
				object = engine.physics.getIntersection(p0, p1, ~0, intersection);
				if (object != NULL)
				{
					radius = length(intersection.getPoint() - p0);
					position = object.getIWorldTransform() * intersection.getPoint();
				}
			} else
			{
				BodyRigid body = object.getBodyRigid();
				if (body != NULL)
				{
					p0 = p0 + normalize(p1 - p0) * radius;
					p1 = object.getWorldTransform() * position;
					float ifps = engine.game.getIFps() / engine.physics.getIFps() * engine.physics.getScale();
					vec3 force = (p0 - p1) * 8.0f / engine.game.getIFps() - body.getWorldVelocity(p1) * 2.0f;
					if (length(force) > 4000.0f)
						force = normalize(force) * 4000.0f;
					body.addWorldForce(p1, force * body.getMass() * ifps * ifps);
					vec4 color = lerp(vec4(0.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), saturate(length(force) / 4000.0f));
					engine.visualizer.renderLine3D(p0, p1, color);
					engine.visualizer.renderSphere(0.2f, translate(p0), color);
					engine.visualizer.renderSphere(0.2f, translate(p1), color);
				}
			}
		} else
			object = NULL;
	} else
		object = NULL;

	return 1;
}
}
