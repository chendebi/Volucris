#include "Scene/scene.h"
#include "Scene/actor.h"
#include "Core/volucris.h"

namespace volucris
{
	Scene::Scene()
	{
	}

	void Scene::addActor(const std::shared_ptr<Actor>& actor)
	{
		if (actor->isAttached())
		{
			V_LOG_WARN(Engine, "actor has been attached");
			return;
		}
		m_actors.push_back(actor);
		actor->attach(this);
	}

	void Scene::tick(const double& delta)
	{
		for (const auto& actor : m_actors)
		{
			actor->tick(delta);
		}
	}

	void Scene::update()
	{
		for (const auto& actor : m_actors)
		{
			actor->update();
		}
	}
}
