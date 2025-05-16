#include "Renderer/viewport_proxy.h"
#include "glad/glad.h"
#include "Core/volucris.h"
#include "Scene/viewport.h"
#include "Scene/scene.h"
#include "Renderer/primitive_proxy.h"
#include "Renderer/render_pass.h"
#include "Renderer/scene_proxy.h"
#include "Renderer/context.h"

namespace volucris
{
	ViewportProxy::ViewportProxy(Viewport* viewport)
		: m_viewport()
		, m_scene(viewport->getScene()->getSceneProxy())
		, m_cameraInfo()
		, m_cameraInfoDirty(true)
	{
	}

	void ViewportProxy::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		V_LOG_DEBUG(Engine, "viewport set to: {}", m_viewport.toString())
	}

	void ViewportProxy::update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives)
	{
		if (m_cameraInfoDirty)
		{
			if (m_cameraInfoBlock.block.valid())
			{
				m_scene->setSceneData(m_cameraInfoBlock, (uint8*) & m_cameraInfo);
			}
			else
			{
				m_cameraInfoBlock = m_scene->addSceneData((uint8*)&m_cameraInfo, sizeof(m_cameraInfo));
			}
		}

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
		context->setCameraInfoBlock(m_cameraInfoBlock);
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
