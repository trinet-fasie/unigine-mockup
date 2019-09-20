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
#include <core/shaders/mesh/common/vertex.h>

STRUCT(FRAGMENT_IN)
	INIT_POSITION
	INIT_DATA(float2, 0, CANVAS_UV)
	INIT_DATA(float2, 1, BRUSH_UV)
END

CBUFFER(parameters)
	UNIFORM float4x4 sb_transform;
END

STRUCT(VERTEX_IN)
	INIT_ATTRIBUTE(float4, 0, POSITION)
	INIT_ATTRIBUTE(float2, 1, TEXCOORD0)
END

MAIN_BEGIN_VERTEX(FRAGMENT_IN)
	
	OUT_POSITION = mul4(sb_transform, getPosition(IN_ATTRIBUTE(0)));
	OUT_POSITION.z = 0.0f;
	
	CANVAS_UV = OUT_POSITION.xy * 0.5f + FLOAT2(0.5f);
#ifdef DIRECT3D11
	CANVAS_UV.y = 1.0f - CANVAS_UV.y;
#endif
	
	BRUSH_UV = IN_ATTRIBUTE(1);
	
MAIN_END
