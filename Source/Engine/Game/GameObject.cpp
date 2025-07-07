#include "Engine/Game/GameObject.h"
#include <Engine/Core/VectorHelp.h>
#include <Core/Assert.h>
#include <Asset/AssetManager.h>
#include <Core/Volucris.h>

namespace volucris
{
	GameObject::GameObject()
		: Object()
		, m_parent(nullptr)
		, m_children()
	{
	}

	GameObject::~GameObject()
	{
		for (const auto& object : m_children)
		{
			v_checkf(object.use_count() == 1, Engine, "object ref count: {}", object.use_count());
			object->m_parent = nullptr;
		}

		if (m_parent)
		{
			setParent(nullptr);
		}
	}

	void GameObject::setParent(GameObject* parent)
	{
		if (m_parent != parent)
		{
			if (m_parent)
			{
				VectorHelp::quickRemove(m_parent->m_children, this);
			}

			m_parent = parent;
			if (m_parent)
			{
				m_parent->m_children.push_back(getShared<GameObject>());
			}
		}
	}

	void GameObject::addDependence(const std::string& path)
	{
		if (AssetManager::getInstance().isPackageRegistered(path))
		{
			m_dependences.push_back(path);
		}
		else
		{
			V_LOG_WARN(Engine, "Package {} is not registered.", path);
		}
	}

	void GameObject::removeDependence(const std::string& path)
	{
		VectorHelp::quickRemove(m_dependences, path);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<GameObject>("GameObject")
			 .constructor<>()
			 .property("Name", &GameObject::getDisplayName, &GameObject::setDisplayName)
			(
				rttr::metadata("Group", "Common"),
				rttr::metadata("Description", "object name")
			);
	}
}
