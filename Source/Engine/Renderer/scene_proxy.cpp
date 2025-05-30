#include "Renderer/scene_proxy.h"
#include "Scene/scene.h"
#include "Renderer/viewport_proxy.h"
#include "Scene/view_client.h"
#include "Renderer/primitive_proxy.h"
#include "Core/vector_help.h"
#include <Renderer/OpenGL/ogl_render_state.h>


namespace volucris
{
	SceneProxy::SceneProxy()
		: m_views()
		, m_primitives()
	{
		m_ubo = std::make_shared<OGLBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	SceneProxy::~SceneProxy()
	{
		
	}

	void SceneProxy::initialize(Scene* scene)
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

	void SceneProxy::addViews(const std::vector<std::shared_ptr<ViewProxy>>& views)
	{
		m_views.insert(m_views.end(), views.begin(), views.end());
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

	UniformBlock SceneProxy::addSceneData(uint8* data, size_t size)
	{
		auto block = m_ubo->addData(data, size);
		return { m_ubo.get(), block };
	}

	void SceneProxy::setSceneData(const UniformBlock& block, uint8* data)
	{
		m_ubo->setBlockData(block.block, data);
	}
}
