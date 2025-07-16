#include "Render/Texture2DProxy.h"
#include <RHI/RHITexture.h>
#include <Render/Renderer.h>

namespace volucris
{
	Texture2DProxy::Texture2DProxy()
		: Object()
		, m_texture(nullptr)
	{
	}

	Texture2DProxy::~Texture2DProxy()
	{
	}

	void Texture2DProxy::setTextureData(const std::vector<uint8>& data)
	{
		m_texture = std::make_shared<RHITexture2D>();
		m_texture->setContext(RHICmdList);
		m_texture->createGpuResource();
		m_texture->init(data);
	}
}
