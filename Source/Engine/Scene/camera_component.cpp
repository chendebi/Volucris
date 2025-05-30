#include "Scene/camera_component.h"
#include <glm/ext.hpp>
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Renderer/viewport_proxy.h"

namespace volucris
{
	CameraComponent::CameraComponent(Mode mode)
		: SceneComponent()
		, m_mode(mode)
		, m_aspect(1200.0/800.0)
		, m_fov(90.f)
		, m_nearDist(0.1f)
		, m_farDist(10000.f)
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
			m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearDist, m_farDist);
		}
		else
		{
			
		}
		m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
		markRenderTransformDirty();
	}

	void CameraComponent::onTransformChanged()
	{
		// todo: 修复这里的bug
		const auto& rot = getRortationTransform();
		auto right = glm::vec3(glm::vec4(1.0, 0.0, 0.0, 1.0));
		auto forward = glm::vec3(glm::vec4(0.0, 0.0, -1.0, 1.0));
		auto eye = getPosition();
		auto center = eye + forward * 0.1f;
		auto up = glm::cross(-forward, right);
		m_viewMatrix = glm::lookAt(eye, center, {0, 1, 0});
		m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void CameraComponent::updateTransform()
	{
		/*if (m_viewport)
		{
			auto proxy = m_viewport->getProxy();
			gApp->getRenderer()->pushCommand([proxy, viewMat = m_viewMatrix, projMat = m_projectionMatrix, pv = m_projectionViewMatrix] {
				proxy->setViewMatrix(viewMat);
				proxy->setProjectionMatrix(projMat);
				proxy->setProjectionViewMatrix(pv);
				});
		}*/
	}

	void CameraComponent::updateRenderState()
	{
		updateTransform();
	}

}
