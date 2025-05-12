#include "Scene/scene_object.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include "Scene/scene.h"

namespace volucris
{
	SceneObject::~SceneObject()
	{
		checkf(!m_scene, Engine, "delete scene object with attached")
	}

	void SceneObject::attach(Scene* scene)
	{
		if (m_scene)
		{
			V_LOG_WARN(Engine, "object {} has been attached to scene", m_displayName);
			return;
		}

		m_scene = scene;
		attached();
		if (m_displayName.empty())
		{
			std::string className = typeid(*this).name();
			auto pos = className.find_last_of(':');
			std::string relClassName;
			if (pos != std::string::npos)
			{
				relClassName = className.substr(pos + 1);
			}
			else
			{
				relClassName = className;
			}
			m_displayName = m_scene->getDefaultDisplayName(relClassName);
		}
		V_LOG_DEBUG(Engine, "object {} attach to scene", m_displayName);
	}

	void SceneObject::disattach()
	{
		if (!m_scene)
		{
			return;
		}

		auto scene = m_scene;
		m_scene = nullptr;
		disattached(scene);

		V_LOG_DEBUG(Engine, "object {} remove from scene", m_displayName);
	}
}
