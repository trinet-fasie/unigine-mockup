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


#define UNDEF_FRAGMENT_OUT

#include <core/shaders/common/fragment.h>

INIT_TEXTURE(0, TEX_DEPTH)
INIT_TEXTURE(1, TEX_ALBEDO)
INIT_TEXTURE(2, TEX_NORMAL)
INIT_TEXTURE(3, TEX_SHADING)
INIT_TEXTURE(4, TEX_COLOR)

STRUCT(FRAGMENT_OUT)
	INIT_MRT(TYPE_RGBA, 0)
	INIT_MRT(TYPE_RGBA, 1)
	INIT_MRT(TYPE_RGBA, 2)
END

#define OUT_ALBEDO		OUT_MRT(0)
#define OUT_NORMAL		OUT_MRT(1)
#define OUT_SHADING		OUT_MRT(2)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float2 uv = IN_POSITION.xy * s_viewport.zw;
	
	float native_depth = TEXTURE_BIAS_ZERO(TEX_DEPTH, uv).r;
	
	float clear = sign(abs(native_depth));
	
	GBuffer gbuffer = GBufferDefault();
	loadGBufferAlbedo(gbuffer, TEXTURE_OUT(TEX_ALBEDO), uv);
	loadGBufferNormal(gbuffer, TEXTURE_OUT(TEX_NORMAL), uv);
	loadGBufferShading(gbuffer, TEXTURE_OUT(TEX_SHADING), uv);
	
	OUT_ALBEDO.rgb = gbuffer.albedo;
	OUT_ALBEDO.a = TEXTURE(TEX_COLOR, uv).a;
	OUT_ALBEDO *= clear;
	
	OUT_NORMAL.rgb = gbuffer.normal * 0.5f + 0.5f;
	OUT_NORMAL.a = 1.0f;
	OUT_NORMAL *= clear;
	
	OUT_SHADING.r = gbuffer.metalness;
	OUT_SHADING.g = gbuffer.roughness;
	OUT_SHADING.b = gbuffer.f0;
	OUT_SHADING.a = gbuffer.microfiber;
	
	OUT_SHADING *= clear;
	
MAIN_END
