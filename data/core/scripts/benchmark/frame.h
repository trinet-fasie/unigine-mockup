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


#ifndef __UNIGINE_FRAME_H__
#define __UNIGINE_FRAME_H__

namespace Unigine
{
class Frame
{
	string set_frame;
	string get_num_frames;

	int duration;

	void run()
	{
		int frame = 0;

		duration = 1000000;

		// parse command line arguments
		forloop(int i = 0; engine.getNumArgs() - 1)
		{
			string arg = engine.getArg(i);
			if (arg == "-frame")
			{
				frame = int(engine.getArg(++i));
				log.message("Frame: %d\n", frame);
			} else if (arg == "-duration")
			{
				duration = int(engine.getArg(++i));
				log.message("Duration: %d\n", duration);
			}
		}

		// disable systems
		engine.gui.setEnabled(0);
		engine.game.setEnabled(0);
		engine.sound.setEnabled(0);
		engine.game.setIFps(1.0f / 30.0f);

		// set frame
		int num_frames = call(get_num_frames);
		log.message("Set %d frame of %d\n", frame, num_frames);
		call(set_frame, frame);

		log.message("Frame started\n");
	}

	void update()
	{
		// duration
		if (duration-- < 0)
		{
			log.message("Frame finished\n");
			engine.console.run("quit");
		}
	}

	Frame(string set_frame_, string get_num_frames_)
	{
		set_frame = set_frame_;
		get_num_frames = get_num_frames_;
	}
};

} /* namespace Unigine */

#endif /* __UNIGINE_FRAME_H__ */
