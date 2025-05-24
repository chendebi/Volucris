#include "Renderer/viewport_proxy.h"
#include "glad/glad.h"
#include "Core/volucris.h"
#include "Scene/viewport.h"
#include "Scene/scene.h"
#include "Renderer/primitive_proxy.h"
#include "Renderer/render_pass.h"
#include "Renderer/scene_proxy.h"
#include "Renderer/context.h"
#include <Application/application.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>

namespace volucris
{
	ViewportProxy::ViewportProxy(Viewport* viewport)
		: m_viewport()
		, m_vp(viewport)
		, m_scene(viewport->getScene()->getSceneProxy())
		, m_cameraInfo()
		, m_cameraInfoDirty(true)
		, m_target()
	{
	}

	void ViewportProxy::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		V_LOG_DEBUG(Engine, "view port size: {}, {}", vp.w, vp.h);
		for (const auto& pass : m_passes)
		{
			pass->viewSizeChanged(vp.w, vp.h);
		}
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

		if (m_target.expired() && !m_passes.empty())
		{
			if (auto tex = (*m_passes.rbegin())->getTargetTexture())
			{
				m_target = tex;
			}
			if (!m_target.expired())
			{
				auto id = m_target.lock()->getID();
				gApp->pushCommand([vp = m_vp, id]() {
					vp->setTargetGLTextureID(id);
					});
			}
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
