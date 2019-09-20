/* Copyright (C) 2005-2019, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.8.0.1 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#ifndef __APP_WORLD_LOGIC_H__
#define __APP_WORLD_LOGIC_H__

#include <UnigineLogic.h>
#include <UnigineStreams.h>

class AppWorldLogic : public Unigine::WorldLogic {
	
public:
	AppWorldLogic();
	virtual ~AppWorldLogic();
	
	virtual int init();
	
	virtual int update();
	virtual int render();
	virtual int flush();
	
	virtual int shutdown();
	virtual int destroy();
	
	virtual int save(const Unigine::StreamPtr &stream);
	virtual int restore(const Unigine::StreamPtr &stream);
};

#endif // __APP_WORLD_LOGIC_H__
