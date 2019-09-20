/* Copyright (C) 2005-2019, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.9 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#ifndef __UNIGINE_SYSTEM_WALL_H__
#define __UNIGINE_SYSTEM_WALL_H__

//#define PROJECTION_USER

#ifdef HAS_APP_WALL || HAS_APP_PROJECTION || HAS_APP_SURROUND
#include <core/scripts/projection.h>
#endif

namespace Wall
{
float fov;
float angle;
float bezel_x;
float bezel_y;

WidgetGroupBox group_gb;
WidgetEditLine fov_el;
WidgetEditLine angle_el;
WidgetEditLine bezel_x_el;
WidgetEditLine bezel_y_el;
WidgetButton projection_b;

string changed_callback;
string changed_world_callback;
string changed_editor_callback;

int user_projection;

void init(float f, float a)
{
	user_projection = 0;
	
	// load config
	fov = engine.config.getFloat("wall_fov", f);
	angle = engine.config.getFloat("wall_angle", a);
	bezel_x = engine.config.getFloat("wall_bezel_x", 0.0f);
	bezel_y = engine.config.getFloat("wall_bezel_y", 0.0f);
	projection_b = projection_b;
	int group_is_hidden = 0;

#ifdef HAS_APP_WALL

	// wall configuration
	int wall_width = engine.wall.getWidth();
	int wall_height = engine.wall.getHeight();
	if (wall_width == 1 && wall_height == 1)
		engine.wall.setPrimary(0, 0);
	else if (wall_width == 1 && wall_height == 2)
		engine.wall.setPrimary(0, 0);
	else if (wall_width == 2 && wall_height == 1)
		engine.wall.setPrimary(0, 0);
	else if (wall_width == 2 && wall_height == 2)
		engine.wall.setPrimary(0, 0);
	else if (wall_width == 3 && wall_height == 1)
		engine.wall.setPrimary(1, 0);
	else if (wall_width == 3 && wall_height == 2)
		engine.wall.setPrimary(1, 0);
	else if (wall_width == 4 && wall_height == 1)
		engine.wall.setPrimary(1, 0);
	else if (wall_width == 5 && wall_height == 1)
		engine.wall.setPrimary(2, 0);
	else
		log.error("Wall::init(): unknown wall configuration %dx%d\n", wall_width, wall_height);

	// update sliders
	if (fov_el != NULL)
		fov_el.setText(format("%.1f", fov));
	if (angle_el != NULL)
		angle_el.setText(format("%.1f", angle));
	if (bezel_x_el != NULL)
		bezel_x_el.setText(format("%.3f", bezel_x));
	if (bezel_y_el != NULL)
		bezel_y_el.setText(format("%.3f", bezel_y));
	if (fov_el != NULL)
		fov_el.setEnabled((wall_width > 1 || wall_height > 1));
	if (angle_el != NULL)
		angle_el.setEnabled((wall_width > 1 || wall_height > 1));
	if (bezel_x_el != NULL)
		bezel_x_el.setEnabled((wall_width > 1));
	if (bezel_y_el != NULL)
		bezel_y_el.setEnabled((wall_height > 1));
	if (projection_b != NULL)
		projection_b.setHidden(1);

#elif HAS_APP_PROJECTION

	// projection configuration
	int projection_width = engine.projection.getWidth();
	if (projection_width == 1)
		engine.projection.setPrimary(0);
	else if (projection_width == 2)
		engine.projection.setPrimary(0);
	else if (projection_width == 3)
		engine.projection.setPrimary(1);
	else if (projection_width == 4)
		engine.projection.setPrimary(1);
	else if (projection_width == 5)
		engine.projection.setPrimary(2);
	else
		log.error("Wall::init(): unknown projection configuration %d\n", projection_width);

	// update sliders
	if (fov_el != NULL)
		fov_el.setText(format("%.1f", fov));
	if (angle_el != NULL)
		angle_el.setText(format("%.1f", angle));
	if (bezel_x_el != NULL)
		bezel_x_el.setText(format("%.3f", bezel_x));
	if (bezel_y_el != NULL)
		bezel_y_el.setText(format("%.3f", bezel_y));
	if (fov_el != NULL)
		fov_el.setEnabled((projection_width > 1));
	if (angle_el != NULL)
		angle_el.setEnabled((projection_width > 1));

#elif HAS_APP_SURROUND

	// update sliders
	if (fov_el != NULL)
		fov_el.setText(format("%.1f", fov));
	if (angle_el != NULL)
		angle_el.setText(format("%.1f", angle));
	if (bezel_x_el != NULL)
		bezel_x_el.setText(format("%.3f", bezel_x));
	if (bezel_y_el != NULL)
		bezel_y_el.setText(format("%.3f", bezel_y));
	if (bezel_y_el != NULL)
		bezel_y_el.setEnabled(0);
	if (projection_b != NULL)
		projection_b.setHidden(1);

#else

	// hide group
	group_is_hidden = 1;

#endif

	group_gb.setHidden(group_is_hidden);

#ifdef HAS_SYNCKER

		//disable elements because syncker duplicate this functionalities
		bezel_x_el.setEnabled(0);
		bezel_y_el.setEnabled(0);
		projection_b.setEnabled(0);

#endif
}

void shutdown()
{
#ifdef HAS_APP_WALL || HAS_APP_PROJECTION || HAS_APP_SURROUND
	// save config
	engine.config.setFloat("wall_fov", fov);
	engine.config.setFloat("wall_angle", angle);
	engine.config.setFloat("wall_bezel_x", bezel_x);
	engine.config.setFloat("wall_bezel_y", bezel_y);
#endif
}

void render()
{
// don't handle projection
#ifdef PROJECTION_USER
	return;
#else
	if (user_projection)
		return;
#endif

#ifdef HAS_APP_WALL || HAS_APP_PROJECTION || HAS_APP_SURROUND

	// get player
	Player player = engine.editor.getPlayer();
	if (player == NULL)
		player = engine.game.getPlayer();
	if (player == NULL)
		return;

	Camera player_camera = player.getCamera();

	// display configuration
	float aspect = float(engine.app.getWidth()) / engine.app.getHeight();

	// player matrices
	mat4 projection = perspective(fov, 1.0f, player.getZNear(), player.getZFar());
	Mat4 modelview = player_camera.getModelview();

#endif

#ifdef HAS_APP_WALL

	// wall configuration
	int wall_width = engine.wall.getWidth();
	int wall_height = engine.wall.getHeight();

	mat4 wall_projection = mat4_identity;
	Mat4 wall_modelview = Mat4_identity;

	forloop(int i = 0; wall_width)
	{
		forloop(int j = 0; wall_height)
		{
			Unigine::getWallProjection(projection, modelview, aspect, wall_width, wall_height, i, j, bezel_x, bezel_y, angle, wall_projection, wall_modelview);
			set_projection(i, j, wall_projection, wall_modelview, player_camera);
		}
	}

#elif HAS_APP_PROJECTION

	// projection configuration
	int projection_width = engine.projection.getWidth();

	mat4 wall_projection = mat4_identity;
	Mat4 wall_modelview = Mat4_identity;

	forloop(int i = 0; projection_width)
	{
		Unigine::getWallProjection(projection, modelview, aspect, projection_width, 1, i, 0, bezel_x, bezel_y, angle, wall_projection, wall_modelview);
		set_projection(i, 0, wall_projection, wall_modelview, player_camera);
	}

#elif HAS_APP_SURROUND

	mat4 wall_projection = mat4_identity;
	Mat4 wall_modelview = Mat4_identity;

	int surround_width = 3;
	int surround_height = 1;

	forloop(int i = 0; surround_width)
	{
		Unigine::getWallProjection(projection, modelview, aspect, surround_width, surround_height, i, 0, bezel_x, bezel_y, angle, wall_projection, wall_modelview);
		set_projection(i, 0, wall_projection, wall_modelview, player_camera);
	}

#endif
}

void set_projection(int x, int y, mat4 projection, Mat4 modelview, Camera camera)
{
#ifdef HAS_APP_WALL

	engine.wall.setEnabled(x, y, 1);
	engine.wall.setProjection(x, y, projection);
	engine.wall.setModelview(x, y, modelview);

	engine.wall.setViewportMask(x, y, camera.getViewportMask());
	engine.wall.setReflectionViewportMask(x, y, camera.getReflectionViewportMask());
	engine.wall.setMaterials(x, y, camera.getPostMaterials());

#elif HAS_APP_PROJECTION

	engine.projection.setEnabled(x, 1);
	engine.projection.setProjection(x, projection);
	engine.projection.setModelview(x, modelview);

	engine.projection.setViewportMask(x, camera.getViewportMask());
	engine.projection.setReflectionViewportMask(x, camera.getReflectionViewportMask());
	engine.projection.setMaterials(x, camera.getPostMaterials());

#elif HAS_APP_SURROUND

	engine.surround.setEnabled(x, 1);
	engine.surround.setProjection(x, projection);
	engine.surround.setModelview(x, modelview);

	engine.surround.setViewportMask(x, camera.getViewportMask());
	engine.surround.setReflectionViewportMask(x, camera.getReflectionViewportMask());
	engine.surround.setMaterials(x, camera.getPostMaterials());

#endif
}

void changed()
{
	if (changed_callback != NULL)
		call(changed_callback);
	if (changed_world_callback != NULL)
		engine.world.call(changed_world_callback);
	if (changed_editor_callback != NULL)
		engine.editor.call(changed_editor_callback);
}

void fov_pressed()
{
	fov = clamp(float(fov_el.getText()), 1.0f, 179.0f);
	fov_el.setText(format("%.1f", fov));
	changed();
}

void angle_pressed()
{
	angle = clamp(float(angle_el.getText()), -89.0f, 89.0f);
	angle_el.setText(format("%.1f", angle));
	changed();
}

void bezel_x_pressed()
{
	bezel_x = clamp(float(bezel_x_el.getText()), -1.0f, 1.0f);
	bezel_x_el.setText(format("%.3f", bezel_x));
	changed();
}

void bezel_y_pressed()
{
	bezel_y = clamp(float(bezel_y_el.getText()), -1.0f, 1.0f);
	bezel_y_el.setText(format("%.3f", bezel_y));
	changed();
}

void projection_clicked()
{
#ifdef HAS_APP_PROJECTION
	engine.projection.showSetupWindow();
#endif
}
}

//////////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////////

void wallInit(float fov = 60.0f, float angle = 20.0f)
{
	Wall::init(fov, angle);
}

void wallShutdown()
{
	Wall::shutdown();
}

void wallRender()
{
	Wall::render();
}

void wallSetFov(float fov)
{
	Wall::fov = fov;
}

float wallGetFov()
{
	return Wall::fov;
}

void wallSetAngle(float angle)
{
	Wall::angle = angle;
}

float wallGetAngle()
{
	return Wall::angle;
}

void wallSetBezelX(float bezel)
{
	Wall::bezel_x = bezel;
}

float wallGetBezelX()
{
	return Wall::bezel_x;
}

void wallSetBezelY(float bezel)
{
	Wall::bezel_y = bezel;
}

float wallGetBezelY()
{
	return Wall::bezel_y;
}

void wallSetChangedCallback(string callback)
{
	Wall::changed_callback = callback;
}

string wallGetChangedCallback()
{
	return Wall::changed_callback;
}

void wallSetChangedWorldCallback(string callback)
{
	Wall::changed_world_callback = callback;
}

string wallGetChangedWorldCallback()
{
	return Wall::changed_world_callback;
}

void wallSetChangedEditorCallback(string callback)
{
	Wall::changed_editor_callback = callback;
}

string wallGetChangedEditorCallback()
{
	return Wall::changed_editor_callback;
}

void wallSetUserProjection(int enabled)
{
	Wall::user_projection = enabled;
}

int wallIsUserProjection()
{
	return Wall::user_projection;
}

#endif /* __UNIGINE_SYSTEM_WALL_H__ */
