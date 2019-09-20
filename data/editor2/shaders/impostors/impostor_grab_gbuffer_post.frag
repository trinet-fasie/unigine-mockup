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
#include <core/shaders/common/fragment.h>

INIT_TEXTURE(0, TEX_DEPTH)
INIT_TEXTURE(1, TEX_ALBEDO)
INIT_TEXTURE(2, TEX_NORMAL)
INIT_TEXTURE(3, TEX_SHADING)
INIT_TEXTURE(4, TEX_BENT_NORMAL)
INIT_TEXTURE(5, TEX_SSAO)

#ifdef GRAB_MODE_DEPTH
	CBUFFER(impostor_grab_parameters)
		UNIFORM float impostor_plane_distance;
	END
#endif

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	OUT_COLOR = float4_one;
	
	// Calculate border mask
	float native_depth = TEXTURE_BIAS_ZERO(TEX_DEPTH, IN_UV).r;
	float clear = sign(abs(native_depth));
	
	float4 clear_color = float4_zero;
	
	GBuffer gbuffer = GBufferDefault();
	
	#ifdef GRAB_MODE_ALBEDO
		loadGBufferAlbedo(gbuffer, TEXTURE_OUT(TEX_ALBEDO), IN_UV);
		OUT_COLOR.rgb = gbuffer.albedo;
	#elif GRAB_MODE_ALBEDO_SSAO
		loadGBufferAlbedo(gbuffer, TEXTURE_OUT(TEX_ALBEDO), IN_UV);
		float ssao = TEXTURE_BIAS_ZERO(TEX_SSAO, IN_UV).r;
		OUT_COLOR.rgb = gbuffer.albedo * ssao;
	#elif GRAB_MODE_NORMAL
		loadGBufferNormal(gbuffer, TEXTURE_OUT(TEX_NORMAL), IN_UV);
		float3 normal = gbuffer.normal;
		normal.g = -normal.g;
		OUT_COLOR.rgb = normal * 0.5f + 0.5f;
		clear_color = float4(0.5f, 0.5f, 1.0f, 0.0f);
	#elif GRAB_MODE_BENT_NORMAL
		float3 normal = TEXTURE_BIAS_ZERO(TEX_BENT_NORMAL, IN_UV).xyz * 2.0f - 1.0f;
		OUT_COLOR.rgb = normal * 0.5f + 0.5f;
		clear_color = float4(0.5f, 0.5f, 1.0f, 0.0f);
	#elif GRAB_MODE_SHADING
		loadGBufferNormal(gbuffer, TEXTURE_OUT(TEX_NORMAL), IN_UV);
		loadGBufferShading(gbuffer, TEXTURE_OUT(TEX_SHADING), IN_UV);
		OUT_COLOR.r = gbuffer.metalness;
		OUT_COLOR.g = gbuffer.roughness;
		OUT_COLOR.b = gbuffer.f0;
		OUT_COLOR.a = gbuffer.microfiber;
	#elif GRAB_MODE_AO
		loadGBufferAlbedo(gbuffer, TEXTURE_OUT(TEX_ALBEDO), IN_UV);
		OUT_COLOR.r = gbuffer.occlusion;
	#elif GRAB_MODE_SSAO
		float ssao = TEXTURE_BIAS_ZERO(TEX_SSAO, IN_UV).r;
		OUT_COLOR.r = ssao;
	#elif GRAB_MODE_TRANSLUCENCE
		loadGBufferShading(gbuffer, TEXTURE_OUT(TEX_SHADING), IN_UV);
		OUT_COLOR.r = gbuffer.translucent;
	#elif GRAB_MODE_DEPTH
		OUT_COLOR.r = impostor_plane_distance - getLinearizedDepth(native_depth, IN_UV);
	#endif
	
	OUT_COLOR = lerp(clear_color, OUT_COLOR, clear);
	
MAIN_END
