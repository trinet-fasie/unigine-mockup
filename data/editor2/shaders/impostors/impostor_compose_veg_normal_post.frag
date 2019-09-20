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

INIT_TEXTURE(0, TEX_RIGHT)
INIT_TEXTURE(1, TEX_LEFT)
INIT_TEXTURE(2, TEX_UP)
INIT_TEXTURE(3, TEX_DOWN)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 right_color = TEXTURE_BIAS_ZERO(TEX_RIGHT, IN_UV);
	float4 left_color = TEXTURE_BIAS_ZERO(TEX_LEFT, IN_UV);
	float4 up_color = TEXTURE_BIAS_ZERO(TEX_UP, IN_UV);
	float4 down_color = TEXTURE_BIAS_ZERO(TEX_DOWN, IN_UV);
	
	OUT_COLOR.r = (right_color.r - left_color.r + 1.0f) / 2.0f;
	OUT_COLOR.g = (down_color.r - up_color.r + 1.0f) / 2.0f;
	OUT_COLOR.b = 0.0f;
	OUT_COLOR.a = right_color.a;
	
MAIN_END
