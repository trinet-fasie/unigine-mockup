#include <core/unigine.h>
#include <core/systems/tracker/editor/tracker_editor.h>

namespace VideoGrabber
{

Unigine::Tracker::Tracker tracker = NULL;
Unigine::Tracker::TrackerTrack track = NULL;

void init()
{
	tracker = new Unigine::Tracker::Tracker();
}

void shutdown()
{
	delete tracker;
	delete track;
}

void load_track(string path)
{
	tracker.reloadTrack(path);
	track = tracker.loadTrack(path);
}

int track_is_loaded()
{
	return track != NULL;
}

int track_set(float time)
{
	track.set(time);
}

int track_get_max_time()
{
	return track.getMaxTime();
}

int track_get_unit_time()
{
	return track.getUnitTime();
}

void reset_track()
{
	if (track != NULL)
		delete track;
	track = NULL;
}

int is_exr_supported()
{
#ifdef HAS_OPENEXR
	return 1;
#else
	return 0;
#endif
}

} // namespace VideoGrabber
