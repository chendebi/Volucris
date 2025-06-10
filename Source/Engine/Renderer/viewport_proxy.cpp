#include "Renderer/viewport_proxy.h"
#include "glad/glad.h"
#include "Core/volucris.h"
#include "Scene/view_client.h"
#include "Scene/scene.h"
#include "Renderer/primitive_proxy.h"
#include "Renderer/render_pass.h"
#include "Renderer/scene_proxy.h"
#include "Renderer/context.h"
#include <Application/application.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Resource/render_target.h>
#include <Renderer/render_target_proxy.h>
#include <Renderer/FRP/forward_render_pass.h>
#include <Renderer/material_proxy.h>

namespace volucris
{
	ViewProxy::ViewProxy()
		: m_scene(nullptr)
		, m_renderTarget(nullptr)
		, m_passes()
		, m_ubo()
		, m_cameraInfo()
		, m_cameraInfoDirty(true)
		, m_client(nullptr)
	{
		addRenderPass(std::make_shared<ForwardRenderPass>());
		m_ubo = std::make_shared<OGLBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	void ViewProxy::initialize(ViewClient* client)
	{
		m_client = client;
		m_scene = client->getScene()->getSceneProxy().get();
		m_renderTarget = client->getRenderTarget()->getProxy();
		(*m_passes.rbegin())->setRenderTarget(m_renderTarget);
	}

	void ViewProxy::update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives)
	{
		if (!m_renderTarget->getViewport().isValid())
		{
			return;
		}

		if (m_renderTarget->isTargetDirty())
		{
			const auto rect = m_renderTarget->getRect();
			for (const auto& pass : m_passes)
			{
				pass->viewSizeChanged(rect.w, rect.h);
			}
		}

		if (m_cameraInfoDirty)
		{
			if (m_cameraInfoBlock.valid())
			{
				m_ubo->setBlockData(m_cameraInfoBlock.block, (uint8*)&m_cameraInfo);
			}
			else
			{
				m_cameraInfoBlock.ubo = m_ubo.get();
				m_cameraInfoBlock.block = m_ubo->addData((uint8*)&m_cameraInfo, sizeof(m_cameraInfo));
			}
			m_cameraInfoDirty = false;
		}

		for (const auto& primitive : primitives)
		{
			for (const auto& batch : primitive->getDrawBatch())
			{
				for (const auto& pass : m_passes)
				{
					pass->addPrimitiveDrawBatch(batch);
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
		if (!m_renderTarget->getViewport().isValid())
		{
			return;
		}
		
		if (m_cameraInfoBlock.valid())
		{
			context->setCameraInfoBlock(&m_cameraInfoBlock);
		}

		for (const auto& pass : m_passes)
		{
			pass->render(context);
		}

		if (m_renderTarget && m_client)
		{
			auto colorTex = m_renderTarget->getOutputColorTexture();
			if (colorTex->getID() > 0)
			{
				uint32 id = colorTex->getID();
				gApp->pushCommand([client = m_client, id]() {
					client->setTargetGLTextureID(id);
					});
				m_client = nullptr;
			}
		}

		context->setCameraInfoBlock(nullptr);
	}

	void ViewProxy::clear()
	{
		for (const auto& pass : m_passes)
		{
			pass->clear();
		}
	}
}
