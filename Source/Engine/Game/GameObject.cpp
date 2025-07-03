#include "Engine/Game/GameObject.h"
#include <Engine/Core/VectorHelp.h>
#include <Core/Assert.h>

namespace volucris
{
	GameObject::GameObject()
		: Object()
		, m_parent(nullptr)
		, m_objects()
	{
	}

	GameObject::~GameObject()
	{
		for (auto object : m_objects)
		{
			v_check(object.use_count() == 1);
		}
	}

	void GameObject::setParent(GameObject* parent)
	{
		if (m_parent != parent)
		{
			if (m_parent)
			{
				VectorHelp::quickRemove(m_parent->m_objects, this);
			}

			m_parent = parent;
			if (m_parent)
			{
				m_parent->m_objects.push_back(getShared<GameObject>());
			}
		}
	}
}
