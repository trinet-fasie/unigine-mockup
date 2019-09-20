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
	UNIFORM float sb_push;
	UNIFORM float sb_radius;
END

INIT_TEXTURE(0, TEX_CANVAS)
INIT_TEXTURE(1, TEX_BRUSH)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 canvas_color = TEXTURE_BIAS_ZERO(TEX_CANVAS, CANVAS_UV);
	
	float4 brush_color = TEXTURE_BIAS_ZERO(TEX_BRUSH, BRUSH_UV);
	brush_color.a = pow(brush_color.a, sb_power) * sb_opacity;
	
#ifdef BLUR_MODE_HORIZONTAL
	float2 uv_offset = float2(s_viewport.z, 0.0f);
#elif BLUR_MODE_VERTICAL
	float2 uv_offset = float2(0.0f, s_viewport.w);
#endif
	uv_offset *= brush_color.a * sb_push;
	
	uint samples = INT(sb_radius);
	float iradius = -1.0f / pow2(sb_radius);
	float weights = 1.0f;
	
	float4 result_color = canvas_color;
	
	for(uint i = 1; i < samples; i++) {
		float weight = exp(iradius * i * i);
		result_color += TEXTURE_BIAS_ZERO(TEX_CANVAS, CANVAS_UV + uv_offset * i) * weight;
		weights += weight;
	}
	
	for(uint i = 1; i < samples; i++) {
		float weight = exp(iradius * i * i);
		result_color += TEXTURE_BIAS_ZERO(TEX_CANVAS, CANVAS_UV - uv_offset * i) * weight;
		weights += weight;
	}
	
	result_color /= weights;
	
	OUT_COLOR = lerp(canvas_color, result_color, sb_channels);
	
MAIN_END
