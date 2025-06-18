#include "Engine/Game/GameObject.h"
#include <Engine/Core/VectorHelp.h>
#include <Core/Assert.h>

namespace volucris
{
	GameObject::GameObject()
		: Object()
		, m_outer(nullptr)
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

	void GameObject::setOuter(GameObject* outer)
	{
		if (m_outer != outer)
		{
			if (m_outer)
			{
				VectorHelp::quickRemove(m_outer->m_objects, this);
			}

			m_outer = outer;
			if (m_outer)
			{
				m_outer->m_objects.push_back(getShared<GameObject>());
			}
		}
	}
}
