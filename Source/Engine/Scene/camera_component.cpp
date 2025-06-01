#include "Scene/camera_component.h"
#include <glm/ext.hpp>
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Renderer/viewport_proxy.h"
#include <Core/volucris.h>

namespace volucris
{
	CameraComponent::CameraComponent(Mode mode)
		: SceneComponent()
		, m_mode(mode)
		, m_aspect(1392/773.0)
		, m_fov(45.f)
		, m_nearDist(0.1f)
		, m_farDist(100.f)
		, m_viewMatrix()
		, m_projectionMatrix()
		, m_projectionViewMatrix()
	{
		updateProjectionMatrix();
		onTransformChanged();
	}

	void CameraComponent::updateProjectionMatrix()
	{
		if (m_mode == PERSPECTIVE)
		{
			m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearDist, m_farDist);
		}
		else
		{
			
		}
		m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
		markRenderTransformDirty();
	}

	void CameraComponent::onTransformChanged()
	{
		auto rotation = getRortationTransform();
		auto upDir = glm::vec4(0, 1, 0, 0);
		auto front = getForwardDirection();
		auto up = glm::normalize(glm::vec3(rotation * upDir));
		m_viewMatrix = glm::lookAt(getPosition(), getPosition() + front, up);
		m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
