#include "Renderer/FRP/forward_render_pass.h"
#include "Renderer/context.h"
#include "Renderer/viewport_proxy.h"

namespace volucris
{
	ForwardRenderPass::ForwardRenderPass()
		: RenderPass()
		, m_clearState()
		, m_view(nullptr)
	{
	}

	void ForwardRenderPass::initialize(ViewportProxy* viewport)
	{
		m_view = viewport;
	}

	void ForwardRenderPass::render(Context* context)
	{
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
}