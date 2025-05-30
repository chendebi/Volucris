#include "Scene/player_actor.h"
#include <Scene/camera_component.h>

namespace volucris
{
	PlayerActor::PlayerActor()
		: Actor()
		, EventHandler()
		, m_cameraComponent(std::make_shared<CameraComponent>())
	{
		addComponent(m_cameraComponent);
	}
}
