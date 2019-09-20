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

INIT_TEXTURE(0, TEX_BG)
INIT_TEXTURE(1, TEX_FG)

MAIN_BEGIN(FRAGMENT_OUT, FRAGMENT_IN)
	
	float4 bg = TEXTURE_BIAS_ZERO(TEX_BG, IN_UV);
	float4 fg = TEXTURE_BIAS_ZERO(TEX_FG, IN_UV);
	
	OUT_COLOR = max(bg, fg);
	
MAIN_END
