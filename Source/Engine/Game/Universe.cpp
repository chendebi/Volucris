#include <Game/Universe.h>
#include <Render/Scene.h>

namespace volucris
{
	Universe::Universe()
		: Object()
		, m_scene()
	{
	}

	Universe::~Universe()
	{

	}

	void Universe::update()
	{

	}

	std::shared_ptr<Scene> Universe::getScene()
	{
		std::shared_ptr<Scene> scene = m_scene.lock();
		if (!scene)
		{
			scene = std::make_shared<Scene>();
			m_scene = scene;
		}
		return scene;
	}
}