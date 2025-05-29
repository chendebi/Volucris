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
#include <Resource/render_target.h>
#include <Renderer/render_target_proxy.h>

namespace volucris
{
	ViewProxy::ViewProxy()
		: m_scene(nullptr)
		, m_renderTarget(nullptr)
		, m_cameraInfo()
		, m_cameraInfoDirty(true)
		, m_target()
	{
	}

	void ViewProxy::initialize(ViewClient* client)
	{
		m_scene = client->getScene()->getSceneProxy();
		m_renderTarget = client->getRenderTarget()->getProxy();
	}

	void ViewProxy::update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives)
	{
		if (m_renderTarget->isTargetDirty())
		{
			//V_LOG_WARN(Engine, "在这里更新pass的渲染目标大小");
			for (const auto& pass : m_passes)
			{
				
			}
		}

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
			m_cameraInfoDirty = false;
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

	void ViewProxy::addRenderPass(const std::shared_ptr<RenderPass>& pass)
	{
		m_passes.push_back(pass);
		pass->initialize(this);
	}

	void ViewProxy::render(Context* context)
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
				/*gApp->pushCommand([vp = m_client, id]() {
					vp->setTargetGLTextureID(id);
					});*/
			}
		}
	}

	void ViewProxy::clear()
	{
		for (const auto& pass : m_passes)
		{
			pass->clear();
		}
	}
}
