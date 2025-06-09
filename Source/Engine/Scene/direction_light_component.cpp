#include "Scene/direction_light_component.h"
#include <Scene/scene.h>
#include <Application/application.h>
#include <Renderer/renderer.h>
#include <Renderer/scene_proxy.h>

namespace volucris
{
	DirectionLightComponent::DirectionLightComponent()
		: Component()
		, m_direction({0, -1, 0, 0})
		, m_color({1,1,1, 1})
		, m_intensity(1.0)
	{
		auto tdir = glm::normalize(glm::vec3(0, -1, -1));
		m_direction = { tdir.x, tdir.y, tdir.z, 0.0 };
	}

	void DirectionLightComponent::updateRenderState()
	{
		auto proxy = getScene()->getSceneProxy();
		DirectionLight light = { m_color, m_direction, m_intensity };
		gApp->getRenderer()->pushCommand([proxy, light]() {
			proxy->setDirectionLightData(light);
			});
	}
}
