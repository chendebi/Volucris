#include "Renderer/scene_proxy.h"
#include "Scene/scene.h"
#include "Renderer/viewport_proxy.h"
#include "Application/viewport.h"
#include "Renderer/primitive_proxy.h"
#include "Core/vector_help.h"

namespace volucris
{
	SceneProxy::SceneProxy(Scene* scene)
		: m_views()
		, m_primitives()
	{

	}

	SceneProxy::~SceneProxy()
	{
		
	}

	void SceneProxy::render(Context* context)
	{
		for (const auto& view : m_views)
		{
			view->update(m_primitives);
			view->render(context);
		}

		// todo: 合并渲染结果

		// 清理这一帧的数据
		for (const auto& view : m_views)
		{
			view->clear();
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

	void SceneProxy::removePrimitiveProxy(PrimitiveProxy* proxy)
	{
		VectorHelp::quickRemoveFirstIf<std::shared_ptr<PrimitiveProxy>>(m_primitives, [proxy](const std::shared_ptr<PrimitiveProxy>& p)->bool {
			return p.get() == proxy;
			});
	}

	void SceneProxy::addRenderPass(const std::shared_ptr<RenderPass>& pass)
	{
		for (const auto& view : m_views)
		{
			view->addRenderPass(pass);
		}
	}
}
