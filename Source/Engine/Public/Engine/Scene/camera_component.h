#ifndef __volucris_camera_component_h__
#define __volucris_camera_component_h__

#include "Engine/Scene/scene_component.h"

namespace volucris
{
	class Viewport;

	class CameraComponent : public SceneComponent
	{
	public:
		enum Mode
		{
			PERSPECTIVE
		};
		
		CameraComponent(Mode mode= PERSPECTIVE);

		void setAspect(float aspect)
		{
			m_aspect = aspect;
			updateProjectionMatrix();
		}

		void setFOV(float fov)
		{
			m_fov = fov;
			updateProjectionMatrix();
		}

		void setNearPlane(float dist)
		{
			m_nearDist = dist;
			updateProjectionMatrix();
		}

		void setFarPlane(float dist)
		{
			m_farDist = dist;
			updateProjectionMatrix();
		}

		glm::mat4 getViewMatrix() const { return m_viewMatrix; }
		glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		glm::mat4 getViewProjectionMatrix() const { return m_projectionViewMatrix; }

	protected:
		void updateProjectionMatrix();

		void onTransformChanged() override;

		void updateTransform() override;

		void updateRenderState() override;

	private:
		Mode m_mode;
		float m_aspect;
		float m_fov;
		float m_nearDist;
		float m_farDist;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_projectionViewMatrix;
	};
}

#endif // !__volucris_camera_component_h__
