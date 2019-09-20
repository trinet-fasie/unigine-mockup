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

INIT_TEXTURE(0, TEX_LIGHTING)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 lighting_color = TEXTURE_BIAS_ZERO(TEX_LIGHTING, IN_UV);
	
	OUT_COLOR = lighting_color;
	
	if (lighting_color.a > 0.0f) {
		OUT_COLOR.rgb = lighting_color.rgb * FLOAT3(0.5f) + FLOAT3(0.5f);
	}
	
MAIN_END
