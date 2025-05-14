#include "simple_pass.h"
#include "Engine/Renderer/context.h"
#include "Engine/Renderer/viewport_proxy.h"

void SimplePass::initialize(ViewportProxy* viewport)
{
	m_view = viewport;
}

void SimplePass::render(Context* context)
{
	context->setViewport(m_view->getViewport());
	context->clear(m_clearState);
	for (const auto& batch : m_renderBatches)
	{
		auto material = batch.material;
		for (const auto& section : batch.sections)
		{
			context->draw2(material, section);
		}
	}
}
