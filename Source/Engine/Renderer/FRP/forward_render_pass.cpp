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
		if (!context->beginRenderPass(m_target.get()))
		{
			return;
		}
		//context->setViewport(m_target->get);
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

	void ForwardRenderPass::setPassTarget(const std::shared_ptr<FrameBufferObject>& target)
	{
		m_target = target;
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