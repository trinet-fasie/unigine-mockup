#include <core/unigine.h>
#include <core/systems/widgets/widget_interface.h>
#include <core/systems/tracker/editor/tracker_editor.h>

Unigine::Tracker::Tracker tracker;
Unigine::Tracker::TrackerEditor track_editor;

Widget tracker_init(Gui gui, WidgetButton load, WidgetButton save, WidgetButton clear)
{
	int flags = Unigine::Tracker::TRACKER_SAVE_RESTORE | Unigine::Tracker::TRACKER_CHECK_OBJECTS;
	if (tracker == NULL)
		tracker = new Unigine::Tracker::Tracker(flags);

	if (track_editor == NULL)
		track_editor = new Unigine::Tracker::TrackerEditor(gui, tracker);

	Unigine::Widgets::Track track = track_editor.getTrackWidget();
	Unigine::Widgets::HBox panel_hbox = track.getPanelHBox();

	panel_hbox.addChild(Unigine::Widgets::widgetCast(load),Unigine::Widgets::ALIGN_RIGHT);
	panel_hbox.addChild(Unigine::Widgets::widgetCast(save),Unigine::Widgets::ALIGN_RIGHT);
	panel_hbox.addChild(Unigine::Widgets::widgetCast(clear),Unigine::Widgets::ALIGN_RIGHT);

	return Unigine::Widgets::Widget(track_editor.getWidget()).widget;
}

void set_viewport_gui(Gui gui)
{
	if (track_editor != NULL)
		track_editor.setViewportGui(gui);
}

void set_viewport_player(Player player)
{
	if (track_editor != NULL)
		track_editor.setViewportPlayer(player);
}

void tracker_shutdown()
{
	delete track_editor;
	delete tracker;
}

void tracker_update()
{
	if (track_editor != NULL)
		track_editor.update();
}

int load_track(string track)
{
	return track_editor.loadTrack(track);
}

int save_track(string track)
{
	return track_editor.saveTrack(track);
}

void clear_track()
{
	track_editor.clear();
}