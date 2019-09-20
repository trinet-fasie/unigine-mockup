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

INIT_TEXTURE(0, TEX_IN)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 in_color = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV);
	
	if (in_color.a > 0.0f) {
		OUT_COLOR = in_color;
	} else {
		float2 north = float2(0.0f, -s_viewport.w);
		float2 south = float2(0.0f, s_viewport.w);
		float2 west = float2(-s_viewport.z, 0.0f);
		float2 east = float2(s_viewport.z, 0.0f);
		
		float4 near_pixels[8];
		near_pixels[0] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + west);
		near_pixels[1] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + north + west);
		near_pixels[2] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + south + west);
		near_pixels[3] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + east);
		near_pixels[4] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + north + east);
		near_pixels[5] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + south + east);
		near_pixels[6] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + north);
		near_pixels[7] = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV + south);
		
		float max_alpha = 0.0f;
		unroll for (uint i = 0; i < 8; ++i) {
			max_alpha = max(max_alpha, near_pixels[i].a);
		}
		
		if (max_alpha > 0.0f) {
			OUT_COLOR = float4_zero;
			for (uint i = 0; i < 8; ++i) {
				OUT_COLOR += (near_pixels[i].a >= max_alpha ? near_pixels[i] : float4_zero);
			}
			
			OUT_COLOR *= max_alpha / OUT_COLOR.a;
		} else {
			OUT_COLOR = in_color;
		}
	}
	
MAIN_END
