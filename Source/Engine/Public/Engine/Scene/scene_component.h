#ifndef __volucris_scene_component_h__
#define __volucris_scene_component_h__

#include "Engine/Scene/component.h"
#include <glm/glm.hpp>

namespace volucris
{
	class SceneComponent : public Component
	{
	public:
		void setPosition(const glm::vec3& pos) { m_position = pos; updateTransform(); }

		glm::vec3 getPosition() const { return m_position; }

		void setRotation(const glm::vec3& rot) { m_rotation = rot; updateTransform(); }

		glm::vec3 getRotation() const { return m_rotation; }

		void setScale(const glm::vec3& scale) { m_scale = scale; updateTransform(); }

		glm::vec3 getScale() const { return m_scale; }

		const glm::mat4& getWorldTransform() const { return m_worldTransform; }

		const glm::mat4& getRortationTransform() const { return m_rotationTransform; }

	protected:
		void updateTransform();

		virtual void onTransformChanged() {}

		void attached() override;

	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
		glm::mat4 m_rotationTransform;
		glm::mat4 m_relativeTransform;
		glm::mat4 m_worldTransform;
		SceneComponent* m_parentSceneComponent;
	};
}

#endif // !__volucris_scene_component_h__
