#include "RHI/RHITexture.h"
#include <glad/glad.h>
#include <RHI/RHICommandList.h>

namespace volucris
{
	static GLint convertToGLFormat(Texture::EPixelFormat format)
	{
		switch (format)
		{
		case volucris::Texture::EPixelFormat::Invalid:
			break;
		case volucris::Texture::EPixelFormat::R8G8B8:
			return GL_RGB;
			break;
		case volucris::Texture::EPixelFormat::R8G8B8A8:
			return GL_RGBA;
			break;
		default:
			break;
		}
		return GL_NONE;
	}

	RHITexture::RHITexture()
		: RHIResource()
	{
	}

	uint32 RHITexture::create(RHICommandList* command)
	{
		uint32 id;
		glGenTextures(1, &id);
		return id;
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
		
	}

	bool RHITexture2D::init(RHICommandList* command)
	{
		command->setTexture(this);
		glTexImage2D(GL_TEXTURE_2D, 0, convertToGLFormat(m_pixelFormat), m_size.width, m_size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		return true;
	}

	void RHITexture2D::bind(RHIState* state)
	{
		if (state->texture2d == this)
		{
			return;
		}
		glBindTexture(GL_TEXTURE_2D, getId());
		state->texture2d = this;
	}

	void RHITexture2D::destroy(RHICommandList* command)
	{
		auto id = getId();
		glDeleteTextures(1, &id);
	}

	std::shared_ptr<RHITexture> RHICreateTexture(const RHITextureDesc& desc)
	{
		switch (desc.texClass)
		{
		case RHITextureDesc::Texture2D:
			return std::make_shared<RHITexture2D>(desc);
			break;
		default:
			break;
		}
		return nullptr;
	}
}
