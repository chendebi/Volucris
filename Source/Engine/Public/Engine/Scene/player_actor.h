#ifndef __volucris_player_actor_h__
#define __volucris_player_actor_h__

#include <Engine/Scene/actor.h>
#include <Engine/Scene/event_handler.h>

namespace volucris
{
	class CameraComponent;
	class InputComponnet;

	class PlayerActor : public Actor, public EventHandler
	{
	public:
		PlayerActor();

		CameraComponent* getCameraComponent() const { return m_cameraComponent.get(); }
		
	private:
		std::shared_ptr<CameraComponent> m_cameraComponent;
	};
}

#endif // !__volucris_player_actor_h__
