#include "RHI/RHITexture.h"
#include <glad/glad.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIOpenGL.h>

namespace volucris
{
	GLenum getTextureInternalFormat(Texture::EPixelFormat format)
	{
		switch (format)
		{
		case volucris::Texture::EPixelFormat::R8G8B8:
			return GL_RGB8;
		case volucris::Texture::EPixelFormat::R8G8B8A8:
			return GL_RGBA8;
		default:
			break;
		}
		return GL_NONE;
	}

	RHITexture::RHITexture()
		: RHIResource()
		, m_type(TextureType::Texture2D)
		, m_id(0)
	{
	}

	RHITexture::~RHITexture()
	{
		if (m_id > 0)
		{
			glDeleteTextures(1, &m_id);
		}
	}

	void RHITexture::createGpuResource()
	{
		glGenTextures(1, &m_id);
	}

	RHITexture2D::RHITexture2D()
		: RHITexture()
		, m_size(0, 0)
		, m_pixelFormat(Texture::EPixelFormat::Invalid)
	{
	}

	RHITexture2D::RHITexture2D(const RHITextureDesc& desc)
		: RHITexture()
		, m_size(desc.size)
		, m_pixelFormat(desc.pixelFormat)
	{
	}

	RHITexture2D::~RHITexture2D()
	{
		getContext()->unsetTexture2D(this);
	}

	bool RHITexture2D::init()
	{
		if (m_pixelFormat == Texture::EPixelFormat::Invalid || !m_size.isValid())
		{
			return false;
		}
		glTexStorage2D(GL_TEXTURE_2D, 1, getTextureInternalFormat(m_pixelFormat), m_size.width, m_size.height);
		GL_CHECK()
		return true;
	}

	void RHITexture2D::update(const Rect& rect, const std::vector<uint8>& data)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width, rect.height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	}

	std::shared_ptr<RHITexture> RHICreateTexture(const RHITextureDesc& desc)
	{
		switch (desc.texClass)
		{
		case TextureType::Texture2D:
			return std::make_shared<RHITexture2D>(desc);
			break;
		default:
			break;
		}
		return nullptr;
	}
}
