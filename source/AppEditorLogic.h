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


#ifndef __APP_EDITOR_LOGIC_H__
#define __APP_EDITOR_LOGIC_H__

#include <UnigineLogic.h>

class AppEditorLogic : public Unigine::EditorLogic {
	
public:
	AppEditorLogic();
	virtual ~AppEditorLogic();
	
	virtual int init();
	
	virtual int update();
	virtual int render();
	
	virtual int shutdown();
	virtual int destroy();
	
	virtual int worldInit();
	virtual int worldShutdown();
	virtual int worldSave();
};

#endif // __APP_EDITOR_LOGIC_H__
