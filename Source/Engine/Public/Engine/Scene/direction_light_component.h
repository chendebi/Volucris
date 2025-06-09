#ifndef __volucris_direction_light_component_h__
#define __volucris_direction_light_component_h__

#include <Engine/Scene/component.h>
#include <glm/glm.hpp>

namespace volucris
{
	class DirectionLightComponent : public Component
	{
	public:
		DirectionLightComponent();

	protected:
		void updateRenderState() override;

	private:
		glm::vec4 m_direction;
		glm::vec4 m_color;
		float m_intensity;
	};
}

#endif // !__volucris_direction_light_component_h__
