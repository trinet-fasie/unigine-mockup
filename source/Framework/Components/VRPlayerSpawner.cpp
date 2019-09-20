#include "VRPlayerSpawner.h"

#include "Players/VRPlayerPC.h"
#include "Players/VRPlayerVive.h"
#include "Players/VRPlayerOculus.h"

REGISTER_COMPONENT(VRPlayerSpawner);

using namespace Unigine;
using namespace Math;

void VRPlayerSpawner::init()
{
	Engine *engine = Engine::get();

	// check Vive VR headset
	if (engine->findPlugin("AppVive") != -1)
	{
		// show vive controls
		vive_basestation_0->setEnabled(1);
		vive_basestation_1->setEnabled(1);
		vive_controller_0->setEnabled(1);
		vive_controller_1->setEnabled(1);
		vive_teleport_marker->setEnabled(1);
		vive_obstacles->setEnabled(0);

		// add node
		PlayerDummyPtr player = PlayerDummy::create();
		init_player(player->getPlayer());
		player->release();

		// add component
		VRPlayerVive* vr_player_vive = ComponentSystem::get()->addComponent<VRPlayerVive>(player->getNode());
		vr_player_vive->basestation_0 = vive_basestation_0;
		vr_player_vive->basestation_1 = vive_basestation_1;
		vr_player_vive->controller_0 = vive_controller_0;
		vr_player_vive->controller_1 = vive_controller_1;
		vr_player_vive->teleport_marker = vive_teleport_marker;
		vr_player_vive->teleport_marker_mat = vive_teleport_marker_mat;
		vr_player_vive->obstacles = vive_obstacles;
		vr_player_vive->hand_force_multiplier = vive_hand_force_multiplier;
		vr_player_vive->controller_buffer_count = vive_controller_buffer_count;
		vr_player = vr_player_vive;
	}
	else
	{
		// hide vive controls
		vive_basestation_0->setEnabled(0);
		vive_basestation_1->setEnabled(0);
		vive_controller_0->setEnabled(0);
		vive_controller_1->setEnabled(0);
		vive_teleport_marker->setEnabled(0);
		vive_obstacles->setEnabled(0);
	}

	// check Oculus VR headset
	if (engine->findPlugin("AppOculus") != -1)
	{
		// show oculus controls
		oculus_warning_message->setEnabled(1);
		oculus_success_message->setEnabled(1);
		oculus_controller_0->setEnabled(1);
		oculus_controller_1->setEnabled(1);
		oculus_teleport_marker->setEnabled(1);
		oculus_obstacles->setEnabled(0);

		// add node
		PlayerDummyPtr player = PlayerDummy::create();
		init_player(player->getPlayer());
		player->release();

		// add component
		VRPlayerOculus* vr_player_oculus = ComponentSystem::get()->addComponent<VRPlayerOculus>(player->getNode());
		vr_player_oculus->warning_message = oculus_warning_message;
		vr_player_oculus->success_message = oculus_success_message;
		vr_player_oculus->controller_0 = oculus_controller_0;
		vr_player_oculus->controller_1 = oculus_controller_1;
		vr_player_oculus->teleport_marker = oculus_teleport_marker;
		vr_player_oculus->teleport_marker_mat = oculus_teleport_marker_mat;
		vr_player_oculus->obstacles = oculus_obstacles;
		vr_player_oculus->hand_force_multiplier = oculus_hand_force_multiplier;
		vr_player_oculus->controller_buffer_count = oculus_controller_buffer_count;
		vr_player = vr_player_oculus;
	}
	else
	{
		// hide oculus controls
		oculus_warning_message->setEnabled(0);
		oculus_success_message->setEnabled(0);
		oculus_controller_0->setEnabled(0);
		oculus_controller_1->setEnabled(0);
		oculus_teleport_marker->setEnabled(0);
		oculus_obstacles->setEnabled(0);
	}

	// if VR is not loaded
	if (engine->findPlugin("AppVive") == -1 &&
		engine->findPlugin("AppOculus") == -1)
	{
		// add node
		PlayerActorPtr player = PlayerActor::create();
		init_player(player->getPlayer());
		player->release();

		// add component
		VRPlayerPC *vr_player_pc = ComponentSystem::get()->addComponent<VRPlayerPC>(player->getNode());
		vr_player_pc->move_speed = move_speed;
		vr_player_pc->acceleration = acceleration;
		vr_player_pc->damping = damping;
		vr_player_pc->hand_length = hand_length;
		vr_player_pc->throw_force = throw_force;
		vr_player = vr_player_pc;
	}
}

void VRPlayerSpawner::init_player(const PlayerPtr &player)
{
	// copy transform
	player->setWorldTransform(node->getWorldTransform());

	if (node->getType() >= Node::PLAYER_BEGIN &&
		node->getType() <= Node::PLAYER_END)
	{
		PlayerPtr player_ref = Player::cast(node);
		
		// copy basic parameters
		player->setFov(player_ref->getFov());
		player->setZNear(player_ref->getZNear());
		player->setZFar(player_ref->getZFar());
		player->setPostMaterials(player_ref->getPostMaterials());
	}

	// unparent current node 
	// (cause vr_group can be our parent and we don't want to turn off
	// components that are children to us)
	node->setWorldParent(NodePtr());
}
