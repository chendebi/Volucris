#include "Renderer/scene_proxy.h"
#include "Scene/scene.h"
#include "Renderer/viewport_proxy.h"
#include "Application/viewport.h"

namespace volucris
{
	SceneProxy::SceneProxy(Scene* scene)
		: m_scene(scene)
		, m_views()
		, m_primitives()
	{
		
	}

	SceneProxy::~SceneProxy()
	{
		
	}

	void SceneProxy::update()
	{

	}

	void SceneProxy::addViewportProxy(const std::shared_ptr<ViewportProxy>& view)
	{
		m_views.push_back(view);
	}

	void SceneProxy::addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy)
	{
		m_primitives.push_back(proxy);
	}
}
