#include "Renderer/scene_proxy.h"
#include "Scene/scene.h"
#include "Renderer/viewport_proxy.h"
#include "Application/viewport.h"
#include "Renderer/primitive_proxy.h"

namespace volucris
{
	SceneProxy::SceneProxy(Scene* scene)
		: m_views()
		, m_primitives()
	{
		setSceneObject(scene);
		scene->setProxy(this);
	}

	SceneProxy::~SceneProxy()
	{
		
	}

	void SceneProxy::update()
	{

	}

	void SceneProxy::render()
	{
		for (const auto& view : m_views)
		{
			view->render();
		}
	}

	void SceneProxy::markSceneObjectRemoved()
	{
		setSceneObject(nullptr);
		for (const auto& view : m_views)
		{
			view->setSceneObject(nullptr);
		}

		for (const auto& primitive : m_primitives)
		{
			primitive->setSceneObject(nullptr);
		}
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
