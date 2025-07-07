#include <Game/Entity.h>
#include <Game/Component.h>
#include <Core/VectorHelp.h>

namespace volucris
{
	Entity::Entity()
		: GameObject()
		, m_components()
	{

	}

	Entity::~Entity()
	{

	}

	void Entity::attach(const std::shared_ptr<Component>& component)
	{
		if (component->m_entity == this)
		{
			return;
		}
		if (component->m_entity)
		{
			component->m_entity->disattach(component);
		}
		component->m_entity = this;
		m_components.emplace_back(component);
	}

	void Entity::disattach(const std::shared_ptr<Component>& component)
	{
		if (component->m_entity != this)
		{
			return;
		}
		VectorHelp::quickRemove(m_components, component);
	}

	void Entity::update()
	{
		for (const auto& comp : m_components)
		{
			comp->update();
		}
	}
}