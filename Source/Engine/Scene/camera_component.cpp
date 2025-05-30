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
		glm::vec3 front;
		auto rot = getRotation();
		front.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		front.y = sin(glm::radians(rot.y));
		front.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		auto Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		auto Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		auto Up = glm::normalize(glm::cross(Right, Front));
		//m_viewMatrix = glm::lookAt(glm::vec3(0.0, 3.0, 5.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		auto eye = getPosition();
		m_viewMatrix = glm::lookAt(eye, eye + Front, Up);
		m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
