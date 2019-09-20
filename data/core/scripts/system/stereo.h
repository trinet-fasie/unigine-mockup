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


#ifndef __UNIGINE_SYSTEM_STEREO_H__
#define __UNIGINE_SYSTEM_STEREO_H__

namespace Stereo
{
float depth;

float distance;
float radius;
float offset;
float angle;
float bezel;

WidgetEditLine distance_el;
WidgetEditLine radius_el;
WidgetEditLine offset_el;
WidgetEditLine angle_el;
WidgetEditLine bezel_el;

void init(float d, float r, float a)
{
	depth = 1.0f;

	// load config
	distance = engine.config.getFloat("stereo_distance", d);
	radius = engine.config.getFloat("stereo_radius", r);
	offset = engine.config.getFloat("stereo_offset", 0.0f);
	angle = engine.config.getFloat("stereo_angle", a);
	bezel = engine.config.getFloat("stereo_bezel", 0.0f);

	// update sliders
	if (distance_el != NULL)
		distance_el.setText(format("%.2f", distance));
	if (radius_el != NULL)
		radius_el.setText(format("%.3f", radius));
	if (offset_el != NULL)
		offset_el.setText(format("%.3f", offset));
	if (angle_el != NULL)
		angle_el.setText(format("%.1f", angle));
	if (bezel_el != NULL)
		bezel_el.setText(format("%.3f", bezel));

	if (angle_el != NULL)
		angle_el.setEnabled(0);
	if (bezel_el != NULL)
		bezel_el.setEnabled(0);
}

void shutdown()
{
	// save config
	engine.config.setFloat("stereo_distance", distance);
	engine.config.setFloat("stereo_radius", radius);
	engine.config.setFloat("stereo_offset", offset);
	engine.config.setFloat("stereo_angle", angle);
	engine.config.setFloat("stereo_bezel", bezel);
}

void update()
{
#ifdef HAS_QUADBUFFER
	if (engine.console.getInt("video_quad_buffer") == 1)
		engine.console.setInt("render_stereo", 5);
#endif

	// set stereo parameters
	engine.render.setStereoDistance(distance);
	engine.render.setStereoRadius(radius * depth);
	engine.render.setStereoOffset(offset);
}

void distance_pressed()
{
	distance = float(distance_el.getText());
	distance_el.setText(format("%.2f", distance));
}

void radius_pressed()
{
	radius = float(radius_el.getText());
	radius_el.setText(format("%.3f", radius));
}

void offset_pressed()
{
	offset = float(offset_el.getText());
	offset_el.setText(format("%.3f", offset));
}

void angle_pressed()
{
	angle = float(angle_el.getText());
	angle_el.setText(format("%.1f", angle));
}

void bezel_pressed()
{
	bezel = float(bezel_el.getText());
	bezel_el.setText(format("%.3f", bezel));
}
}

//////////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////////

void stereoInit(float distance = 4.0f, float radius = 0.032f, float angle = 20.0f)
{
	Stereo::init(distance, radius, angle);
}

void stereoShutdown()
{
	Stereo::shutdown();
}

void stereoUpdate()
{
	Stereo::update();
}

void stereoRender()
{
	// we keep this function for compatibility with old user projects
	// it's empty since version 2.2
}

#endif /* __UNIGINE_SYSTEM_STEREO_H__ */
