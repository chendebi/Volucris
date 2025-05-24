#include "Renderer/FRP/forward_render_pass.h"
#include "Renderer/context.h"
#include "Renderer/viewport_proxy.h"
#include <Renderer/OpenGL/ogl_frame_buffer_object.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Renderer/OpenGL/ogl_render_buffer_object.h>

namespace volucris
{
	ForwardRenderPass::ForwardRenderPass()
		: RenderPass()
		, m_clearState()
		, m_view(nullptr)
		, m_target(nullptr)
	{
		m_clearState.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	}

	void ForwardRenderPass::initialize(ViewportProxy* viewport)
	{
		m_view = viewport;
		const auto& vp = viewport->getViewport();
		viewSizeChanged(vp.w, vp.h);
	}

	void ForwardRenderPass::render(Context* context)
	{
		if (!context->beginRenderPass(m_target.get()))
		{
			return;
		}
		context->setViewport(m_view->getViewport());
		context->clear(m_clearState);
		for (const auto& batch : m_renderBatches)
		{
			auto material = batch.material;
			for (const auto& section : batch.sections)
			{
				context->draw(material, section);
			}
		}
	}

	void ForwardRenderPass::viewSizeChanged(int width, int height)
	{
		if (!m_target)
		{
			m_target = std::make_shared<FrameBufferObject>();

			auto colorTex = std::make_shared<Texture2DObject>();
			colorTex->setFormat(GL_RGB);
			colorTex->setType(GL_UNSIGNED_BYTE);
			m_target->attachColor(0, colorTex);

			auto depthRbo = std::make_shared<RenderBufferObject>();
			depthRbo->setFormat(GL_DEPTH24_STENCIL8);
			m_target->attachDepth(depthRbo);
		}
		auto colorTex = m_target->getColorAttachment(0);
		colorTex->setSize(width, height);
		m_target->getDepthAttachment()->setSize(width, height);
	}

	std::shared_ptr<Texture2DObject> ForwardRenderPass::getTargetTexture() const
	{
		if (m_target)
		{
			return std::dynamic_pointer_cast<Texture2DObject>(m_target->getColorAttachment(0));
		}
		return nullptr;
	}
}