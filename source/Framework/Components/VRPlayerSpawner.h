#pragma once
#include <UniginePlayers.h>
#include "../../ComponentSystem/ComponentSystem.h"
#include "Players/VRPlayer.h"

// spawns VRPlayer at node's position
class VRPlayerSpawner : public ComponentBase
{
public:
	COMPONENT(VRPlayerSpawner, ComponentBase);
	COMPONENT_INIT(init);

	// property
	PROP_NAME("vr_player_spawner");
	PROP_AUTOSAVE(0);

	// VIVE group
	PROP_PARAM(Node, vive_basestation_0);
	PROP_PARAM(Node, vive_basestation_1);
	PROP_PARAM(Node, vive_controller_0);
	PROP_PARAM(Node, vive_controller_1);
	PROP_PARAM(Node, vive_teleport_marker);
	PROP_PARAM(Material, vive_teleport_marker_mat);
	PROP_PARAM(Node, vive_obstacles);
	PROP_PARAM(Float, vive_hand_force_multiplier, 1.3f);	// getHandLinearVelocity = velocity * hand_force_multiplier
	PROP_PARAM(Int, vive_controller_buffer_count, 4);

	// OCULUS group
	PROP_PARAM(Node, oculus_warning_message);
	PROP_PARAM(Node, oculus_success_message);
	PROP_PARAM(Node, oculus_controller_0);
	PROP_PARAM(Node, oculus_controller_1);
	PROP_PARAM(Node, oculus_teleport_marker);
	PROP_PARAM(Material, oculus_teleport_marker_mat);
	PROP_PARAM(Node, oculus_obstacles);
	PROP_PARAM(Float, oculus_hand_force_multiplier, 1.3f);	// getHandLinearVelocity = velocity * hand_force_multiplier
	PROP_PARAM(Int, oculus_controller_buffer_count, 4);

	// PC group
	PROP_PARAM(Float, move_speed, 2.5f);	// speed of the player
	PROP_PARAM(Float, acceleration, 8.0f);	// acceleration of the player
	PROP_PARAM(Float, damping, 8.0f);		// damping of the player
	PROP_PARAM(Float, hand_length, 1.3f);	// uses for grab objects
	PROP_PARAM(Float, throw_force, 7.0f);	// start velocity of grabbed object

	// return spawned player's component
	UNIGINE_INLINE VRPlayer* getSpawnedPlayer() const { return vr_player; }

protected:
	void init();

private:
	void init_player(const Unigine::PlayerPtr &player);

	VRPlayer* vr_player;
};
