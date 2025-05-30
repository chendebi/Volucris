#include "Scene/scene_component.h"
#include <glm/ext/matrix_transform.hpp>

namespace volucris
{
	SceneComponent::SceneComponent()
		: Component()
		, m_position()
		, m_rotation()
		, m_scale()
		, m_rotationTransform()
		, m_relativeTransform()
		, m_worldTransform()
		, m_parentSceneComponent(nullptr)
	{
		setScale({ 1.f, 1.f, 1.f });
	}

	void SceneComponent::transformChanged()
	{
		const glm::vec3 xasix = { 1.0, 0.0, 0.0 };
		const glm::vec3 yasix = { 0.0, 1.0, 0.0 };
		const glm::vec3 zasix = { 0.0, 0.0, 1.0 };
		m_relativeTransform = glm::scale(glm::mat4(1.0), m_scale);

		m_rotationTransform = glm::rotate(glm::mat4(1.0), glm::radians(m_rotation.y), yasix);
		m_rotationTransform = glm::rotate(m_rotationTransform, glm::radians(m_rotation.x), xasix);
		m_rotationTransform = glm::rotate(m_rotationTransform, glm::radians(m_rotation.z), zasix);

		m_relativeTransform = m_rotationTransform * m_rotationTransform;
		m_relativeTransform = glm::translate(m_relativeTransform, m_position);

		if (m_parentSceneComponent)
		{
			m_worldTransform = m_parentSceneComponent->getWorldTransform() * m_relativeTransform;
		}
		else
		{
			m_worldTransform = m_relativeTransform;
		}
		onTransformChanged();
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
