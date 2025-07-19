#ifndef __volucris_region_h__
#define __volucris_region_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Entity.h>

namespace volucris
{
	class GameWorld;

	class Region : public GameObject
	{
	public:
		Region();

		void addEntity(std::unique_ptr<Entity> entity)
		{
			m_entites.push_back(std::move(entity));
		}

		void update();

	private:
		GameWorld* m_world;
		std::vector<std::unique_ptr<Entity>> m_entites;
	};
}

#endif // !__volucris_region_h__
