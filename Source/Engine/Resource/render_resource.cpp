#include "Resource/render_resource.h"
#include "Core/assert.h"
#include "Core/volucris.h"

volucris::RenderResource::~RenderResource()
{
	checkf(m_renderRef == 0, Engine, "resource release error: {}", getResourceFullPath())
}

void volucris::RenderResource::removeRenderRef()
{
	check(m_renderRef > 0)
	--m_renderRef;
	if (m_renderRef == 0)
	{
		releaseRenderProxy();
	}
}
