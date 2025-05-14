#include "Renderer/viewport_proxy.h"
#include "glad/glad.h"
#include "Core/volucris.h"
#include "Application/viewport.h"
#include "Scene/scene.h"
#include "Renderer/primitive_proxy.h"
#include "Renderer/render_pass.h"

namespace volucris
{
	ViewportProxy::ViewportProxy(Viewport* viewport)
		: m_viewport()
		, m_scene(viewport->getScene()->getSceneProxy())
	{
	}

	void ViewportProxy::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		V_LOG_DEBUG(Engine, "viewport set to: {}", m_viewport.toString())
	}

	void ViewportProxy::update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives)
	{
		for (const auto& primitive : primitives)
		{
			for (const auto& batch : primitive->getDrawBatch())
			{
				for (const auto& pass : m_passes)
				{
					pass->addBatch(batch);
				}
			}
		}
	}

	void ViewportProxy::addRenderPass(const std::shared_ptr<RenderPass>& pass)
	{
		m_passes.push_back(pass);
		pass->initialize(this);
	}

	void ViewportProxy::render(Context* context)
	{
		for (const auto& pass : m_passes)
		{
			pass->render(context);
		}
	}

	void ViewportProxy::clear()
	{
		for (const auto& pass : m_passes)
		{
			pass->clear();
		}
	}
}
