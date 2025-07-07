#include "Game/Component.h"
#include <Game/Entity.h>

namespace volucris
{
	Component::Component()
		: GameObject()
		, m_dirtyFlags(0)
	{
	}

	void Component::attachTo(Entity* entity)
	{
		if (entity == m_entity)
		{
			return;
		}

		auto shared = getShared<Component>();
		if (m_entity)
		{
			m_entity->disattach(shared);
		}

		m_entity = entity;
		if (m_entity)
		{
			m_entity->attach(shared);
		}
	}

	void Component::update()
	{
		if (m_dirtyFlags & DirtyFlag::RenderState)
		{
			onRenderStateChanged();
		}
		if (m_dirtyFlags & DirtyFlag::TransformState)
		{
			onTransformStateChanged();
		}
		m_dirtyFlags = 0;
	}
} // namespace volucris

BOOST_CLASS_EXPORT(volucris::Component)