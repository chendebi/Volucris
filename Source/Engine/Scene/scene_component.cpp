#include "Scene/scene_component.h"
#include <glm/ext/matrix_transform.hpp>

namespace volucris
{
	void SceneComponent::updateTransform()
	{
		const glm::vec3 xasix = { 1.0, 0.0, 0.0 };
		const glm::vec3 yasix = { 0.0, 1.0, 0.0 };
		const glm::vec3 zasix = { 0.0, 0.0, 1.0 };
		m_rotationTransform = glm::mat4(1.0);
		glm::scale(m_rotationTransform, m_scale);
		glm::rotate(m_rotationTransform, glm::radians(m_rotation.x), xasix);
		glm::rotate(m_rotationTransform, glm::radians(m_rotation.y), yasix);
		glm::rotate(m_rotationTransform, glm::radians(m_rotation.z), zasix);
		m_relativeTransform = m_rotationTransform;
		glm::translate(m_relativeTransform, m_position);

		if (m_parentSceneComponent)
		{
			m_worldTransform = m_parentSceneComponent->getWorldTransform() * m_relativeTransform;
		}
		else
		{
			m_worldTransform = m_relativeTransform;
		}

		markRenderTransformDirty();
	}

	void SceneComponent::attached()
	{
		Component::attached();
		auto comp = getParentComponent();
		while (comp)
		{
			if (auto parentSceneComp = dynamic_cast<SceneComponent*>(comp))
			{
				m_parentSceneComponent = parentSceneComp;
				break;
			}
			comp = comp->getParentComponent();
		}
	}
}
