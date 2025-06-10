#include "Renderer/FRP/forward_render_pass.h"
#include "Renderer/context.h"
#include "Renderer/viewport_proxy.h"
#include <Renderer/OpenGL/ogl_frame_buffer_object.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Renderer/OpenGL/ogl_render_buffer_object.h>
#include <Renderer/render_target_proxy.h>
#include <Renderer/OpenGL/ogl_check.h>

namespace volucris
{
	ForwardRenderPass::ForwardRenderPass()
		: RenderPass()
		, m_clearState()
	{
		m_clearState.color = glm::vec4(0.0, 0.0, 0.0, 1.0);
	}

	void ForwardRenderPass::initialize(ViewProxy* viewport)
	{
		/*m_view = viewport;
		const auto& vp = viewport->getViewport();
		viewSizeChanged(vp.w, vp.h);*/
	}

	void ForwardRenderPass::render(Context* context)
	{
		GL_CHECK();
		if (!context->beginRenderPass(m_renderTarget->getFrameBuffer().get()))
		{
			return;
		}
		GL_CHECK();
		context->clear(m_clearState);
		context->setViewport(m_renderTarget->getViewport());
		GL_CHECK();
		for (const auto& batch : m_renderBatches)
		{
			auto material = batch.material;
			for (const auto& section : batch.sections)
			{
				context->draw(material, batch.primitiveInfo, section);
				GL_CHECK();
			}
		}
	}
}