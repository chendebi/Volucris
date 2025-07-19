#include "Game/GameWorld.h"
#include <Render/Scene.h>

namespace volucris
{
	GameWorld::GameWorld()
		: World()
		, m_persistentRegion(std::make_unique<Region>())
		, m_regions()
		, m_scene()
	{
	}

	std::shared_ptr<Scene> GameWorld::getScene()
	{
		std::shared_ptr<Scene> scene = nullptr;
		if (!m_scene.expired())
		{
			scene = m_scene.lock();
		}

		if (scene == nullptr)
		{
			scene = std::make_shared<Scene>();
			m_scene = scene;
		}

		return scene;
	}

	void GameWorld::update()
	{
		m_persistentRegion->update();
		for (const auto& region : m_regions)
		{
			region->update();
		}
	}
}
