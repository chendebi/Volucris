#ifndef __volucris_world_h__
#define __volucris_world_h__

#include <vector>
#include <memory>
#include <Engine/Game/GameObject.h>

namespace volucris
{
	class Package;

	class World : public GameObject
	{
	public:
		World();

		~World() override;

		void addPackage(const std::shared_ptr<Package>& package)
		{
			m_packages.push_back(package);
		}

	private:
		std::vector<std::shared_ptr<Package>> m_packages;
	};
}

#endif // !__volucris_world_h__
