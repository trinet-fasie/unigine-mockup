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


#ifndef __UNIGINE_TRACKER_PARAMETER_SWITCH_H__
#define __UNIGINE_TRACKER_PARAMETER_SWITCH_H__

namespace Unigine::Tracker
{
class TrackerParameterSwitch : TrackerParameter
{
	int min_value; // minimum value
	int max_value; // maximum value

	class Track
	{
		KeyInt keys[0];   // keys
		float indices[0]; // indices
		int cache;		  // cache
	};

	TrackerParameterSwitch()
		: TrackerParameter(TYPE_SWITCH)
	{
		min_value = -1000000;
		max_value = 1000000;
	}

	int getMinValue()
	{
		return min_value;
	}

	int getMaxValue()
	{
		return max_value;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Load parameter
	//////////////////////////////////////////////////////////////////////////

	int load(Xml xml)
	{
		if (TrackerParameter::load(xml) == 0)
			return 0;

		// value bounds
		if (xml.isArg("min"))
			min_value = xml.getIntArg("min");
		if (xml.isArg("max"))
			max_value = xml.getIntArg("max");

		return 1;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Compile parameter
	//////////////////////////////////////////////////////////////////////////

	string getBlendSource()
	{
		string source = "int VALUE;\n";
		source += get_source;
		source += "VALUE = int(lerp(VALUE,Unigine::Tracker::TrackerParameterSwitch::getTrackValue(track,time),k));\n";
		source += set_source;
		return source;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Track
	//////////////////////////////////////////////////////////////////////////

	void getTrackKeys(Track track, float keys[])
	{
		keys.copy(track.indices);
	}

	int getNumTrackKeys(Track track)
	{
		return track.keys.size();
	}

	float getPrevTrackTime(Track track, float time)
	{
		return get_prev_track_time(track.indices, time);
	}

	float getNextTrackTime(Track track, float time)
	{
		return get_next_track_time(track.indices, time);
	}

	void updateTrack(Track track)
	{
		// update track
		update_track_int(track.keys, track.indices);

		// unused cache
		track.cache = track.cache;
	}

	void setTrackValue(Track track, float time, int value)
	{
		if (getTrackValue(track, time) != value)
		{
			set_track_value_int(track.keys, track.indices, time, value);
			updateTrack(track);
		}
	}

	int getTrackValue(Track track, float time)
	{
		return get_track_value_int(track.keys, track.indices, time);
	}

	void saveTrackValue(Stream stream, int value)
	{
		stream.writeInt(value);
	}

	int restoreTrackValue(Stream stream)
	{
		return stream.readInt();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Clone track
	//////////////////////////////////////////////////////////////////////////

	Track cloneTrack(Track track)
	{
		Track t = new Track();

		// clone track
		clone_track_int(t.keys, track.keys);
		updateTrack(t);

		return t;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Load/save track
	//////////////////////////////////////////////////////////////////////////

	int loadTrack(Xml xml, Track &track, int &object, int &argument)
	{
		// load track
		track = new Track();
		forloop(int i = 0; xml.getNumChildren())
		{
			Xml x = xml.getChild(i);
			string name = x.getName();

			// key
			if (name == "key")
			{
				// key time
				if (x.isArg("time") == 0)
				{
					log.error("Unigine::Tracker::TrackerParameterSwitch::loadTrack(): can't find key time\n");
					delete track;
					return 0;
				}

				// create key
				KeyInt key = new KeyInt();
				key.time = x.getFloatArg("time");

				// key value
				key.value = x.getIntData();

				// append key
				track.keys.append(key);
			}

			else
			{
				log.error("Unigine::Tracker::TrackerParameterSwitch::loadTrack(): unknown node \"%s\"\n", name);
				continue;
			}
		}
		updateTrack(track);

		if (TrackerParameter::loadTrack(xml, track, object, argument) == 0)
		{
			delete track;
			return 0;
		}

		return 1;
	}

	int saveTrack(Xml xml, Track track, int object, int argument)
	{
		if (TrackerParameter::saveTrack(xml, track, object, argument) == 0)
			return 0;

		// save track
		forloop(int i = 0; track.keys.size())
		{
			Xml x = xml.addChild("key");
			KeyInt key = track.keys[i];

			// key time
			x.setFloatArg("time", key.time);

			// key value
			x.setIntData(key.value);
		}

		return 1;
	}
};

} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_SWITCH_H__ */