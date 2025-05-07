#include "Scene/actor.h"
#include "Core/volucris.h"
#include "Scene/scene_component.h"

namespace volucris
{
	Actor::Actor()
	{
		setRootComponent(std::make_shared<SceneComponent>());
	}

	bool Actor::addComponent(const std::shared_ptr<Component>& component)
	{
		if (component->getParentComponent())
		{
			V_LOG_WARN(Engine, "todo: support dynamic change component parent.");
			return false;
		}
		m_rootComponent->addChild(component);
		return true;
	}

	void Actor::update()
	{
		m_rootComponent->update();
		onUpdate();
	}
}
