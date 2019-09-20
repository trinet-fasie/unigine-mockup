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
INIT_TEXTURE_ARRAY(1, TEX_LIGHTS)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	// Calculate border mask
	float native_depth = TEXTURE_BIAS_ZERO(TEX_DEPTH, IN_UV).r;
	float alpha = sign(abs(native_depth));
	
	float4 lighting_color = TEXTURE_ARRAY_FETCH(TEX_LIGHTS, IN_POSITION.xy, 0);
	lighting_color.rgb = srgb(lighting_color.rgb);
	float lighting_grayscale = max3(lighting_color);
	
	OUT_COLOR = float4(lighting_grayscale, 0.0f, 0.0f, alpha);
	
MAIN_END
