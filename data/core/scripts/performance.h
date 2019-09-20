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


namespace Unigine {

	class PerformanceFrame {
		float ftime;
		float total;
		float update;
		float render_cpu;
		float waiting_gpu;
		float interface;
		float physics
	};

	void performanceWarmUp(int warm_frames, int warm_seconds) {
		float timer = 0;
		forloop(int i = 0; warm_frames) {
			timer += engine.game.getFTime();
			wait;
		}

		if(timer < warm_seconds) {
			int additional_time = warm_frames * warm_seconds / timer;
			forloop(int j = 0; additional_time) wait;
		}
	}

	void performanceGetData(int frames, PerformanceFrame data[]) {
		data.resize(frames);

		forloop(int i = 0; frames) {
			PerformanceFrame frame = new PerformanceFrame();
			frame.ftime = engine.game.getFTime();
			frame.total = engine.getTotalTime();
			frame.update = engine.getUpdateTime();
			frame.render_cpu = engine.getRenderTime();
			frame.waiting_gpu = engine.getPresentTime();
			frame.interface = engine.getInterfaceTime();
			frame.physics = engine.physics.getTotalTime();
			data[i] = frame;
			wait;
		}
	}

	int performanceWriteData(string name, PerformanceFrame data[]) {
		File f = new File(name, "w");
		if (!f.isOpened()) {
			delete f;
			return false;
		}

		forloop(int i = 0; data.size()) {
			PerformanceFrame frame = data[i];
			f.puts(format("%f,%f,%f,%f,%f,%f,%f\n",frame.ftime,frame.total,frame.update,frame.render_cpu,frame.waiting_gpu,frame.interface,frame.physics));
		}

		f.close();
		delete f;
	}

	void performanceCapture(string name, int warm_frames, int warm_seconds, int profiler_frames) {
		#ifdef ENABLE_PERFORMANCE_CAPTURE
			engine.console.run("show_profiler 2");
			engine.console.run("show_fps 0");
			engine.gui.setHidden(1);

			while(engine.world.isLoaded() == 0) wait;

			performanceWarmUp(warm_frames, warm_seconds);

			PerformanceFrame data[0] = ();
			performanceGetData(profiler_frames,data);
			performanceWriteData(name, data);
			engine.console.run("quit");
		#else
			engine.console.run("show_profiler 2");
			engine.console.run("show_fps 2");
			engine.gui.setHidden(0);
		#endif
	}

	void performanceCaptureThread(string name = "performance.log", int warm_frames = 300, int warm_seconds = 5, int profiler_frames = 600) {
		thread("performanceCapture", name, warm_frames, warm_seconds, profiler_frames);
		yield 1;
	}
}
