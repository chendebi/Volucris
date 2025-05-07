#ifndef __volucris_scene_h__
#define __volucris_scene_h__

#include <memory>
#include <vector>

namespace volucris
{
	class Actor;

	class Scene
	{
	public:
		Scene();

		void addActor(const std::shared_ptr<Actor>& actor);

		void tick(const double& delta);

		void update();

	private:
		std::vector<std::shared_ptr<Actor>> m_actors;
	};
}

#endif // !__volucris_scene_h__
