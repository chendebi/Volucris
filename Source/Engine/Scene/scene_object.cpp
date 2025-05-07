#include "Scene/scene_object.h"
#include "Core/volucris.h"
#include <Core/assert.h>

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
			V_LOG_WARN(Engine, "object has been attached");
			return;
		}
		m_scene = scene;
		attached();
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
	}
}
