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


#include <core/shaders/mesh/common/common.h>
#include <core/shaders/mesh/common/fragment.h>

STRUCT(FRAGMENT_IN)
	INIT_POSITION
	INIT_DATA(float2, 0, CANVAS_UV)
	INIT_DATA(float2, 1, BRUSH_UV)
END

CBUFFER(parameters)
	UNIFORM float sb_opacity;
	UNIFORM float sb_power;
	UNIFORM float4 sb_channels;
	UNIFORM float sb_intensity;
	UNIFORM float4 sb_color;
#ifdef DRAW_MODE_ADD || DRAW_MODE_SUBTRACT
	UNIFORM float sb_push;
#endif
END

INIT_TEXTURE(0, TEX_CANVAS)
INIT_TEXTURE(1, TEX_BRUSH)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 canvas_color = TEXTURE_BIAS_ZERO(TEX_CANVAS, CANVAS_UV);
	
	float4 brush_color = TEXTURE_BIAS_ZERO(TEX_BRUSH, BRUSH_UV);
	brush_color.rgb = brush_color.rgb * sb_color.rgb * sb_intensity;
	brush_color.a = pow(brush_color.a, sb_power) * sb_opacity;
	
	float4 result_color;
#ifdef DRAW_MODE_BLEND
	result_color.rgb = lerp(canvas_color.rgb, brush_color.rgb, brush_color.a);
	result_color.a = lerp(canvas_color.a, sb_color.a, brush_color.a);
#elif DRAW_MODE_ADD
	brush_color.rgb *= brush_color.a;
	result_color = canvas_color + brush_color * sb_push;
#elif DRAW_MODE_SUBTRACT
	brush_color.rgb *= brush_color.a;
	result_color = canvas_color - brush_color * sb_push;
#endif
	
	OUT_COLOR = lerp(canvas_color, result_color, sb_channels);
	
MAIN_END
