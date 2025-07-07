#ifndef __volucris_entity_h__
#define __volucris_entity_h__

#include <Engine/Game/GameObject.h>

namespace volucris
{
	class Component;

	class Entity : public GameObject
	{
	public:
		Entity();

		~Entity() override;

		void attach(const std::shared_ptr<Component>& component);

		void disattach(const std::shared_ptr<Component>& component);

		void update();

	private:
		std::vector<std::shared_ptr<Component>> m_components;
	};
}

#endif // !__volucris_entity_h__
