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
INIT_TEXTURE(1, TEX_CHANNEL)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 in_color = TEXTURE_BIAS_ZERO(TEX_IN, IN_UV);
	float4 channel_color = TEXTURE_BIAS_ZERO(TEX_CHANNEL, IN_UV);
	
	#ifdef IN_CHANNEL_R
		float channel = channel_color.r;
	#elif IN_CHANNEL_G
		float channel = channel_color.g;
	#elif IN_CHANNEL_B
		float channel = channel_color.b;
	#elif IN_CHANNEL_A
		float channel = channel_color.a;
	#endif
	
	OUT_COLOR = in_color;
	
	#ifdef OUT_CHANNEL_R
		OUT_COLOR.r = channel;
	#elif OUT_CHANNEL_G
		OUT_COLOR.g = channel;
	#elif OUT_CHANNEL_B
		OUT_COLOR.b = channel;
	#elif OUT_CHANNEL_A
		OUT_COLOR.a = channel;
	#endif
	
MAIN_END
