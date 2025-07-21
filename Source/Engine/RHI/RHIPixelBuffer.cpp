#include <RHI/RHIPixelBuffer.h>
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHIOpenGLBuffer.h>
#include <RHI/RHITexture.h>
#include <RHI/RHICommandList.h>

namespace volucris
{
	static GLenum getGLReadFormat(Texture::EPixelFormat format)
	{
		switch (format)
		{
		case volucris::Texture::EPixelFormat::R8G8B8:
			return GL_RGB;
		case volucris::Texture::EPixelFormat::R8G8B8A8:
			return GL_RGBA;
		default:
			break;
		}
		return GL_NONE;
	}

	RHIReadPixelBuffer::RHIReadPixelBuffer(RHIBuffer::Usage usage)
		: RHIBuffer(RHIBuffer::PixelPackBuffer, usage)
		, m_texture(nullptr)
	{
	}

	void RHIReadPixelBuffer::startRead(Rect rect)
	{
		if (!m_texture)
		{
			return;
		}
		auto ctx = getContext();
		ctx->setBuffer(this);
		ctx->setTexture2D(m_texture.get());
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(rect.x, rect.y, rect.width, rect.height, getGLReadFormat(m_texture->getPixelFormat()), GL_UNSIGNED_BYTE, 0);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		GL_CHECK();
	}

	std::vector<uint8> RHIReadPixelBuffer::readColor()
	{
		getContext()->setBuffer(this);
		void* ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if (ptr == nullptr)
		{
			V_LOG_ERROR(Engine, "Failed to map pixel buffer for reading color data.")
				return {};
		}
		std::vector<uint8> colorData(m_buffer->size);
		memcpy(colorData.data(), ptr, m_buffer->size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		return colorData;
	}


	bool RHIReadPixelBuffer::readColorTo(std::vector<uint8>& data)
	{
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		void* ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if (ptr == nullptr)
		{
			V_LOG_ERROR(Engine, "Failed to map pixel buffer for reading color data.")
				return false;
		}

		if (data.size() != m_buffer->size)
		{
			return false;
		}
		memcpy(data.data(), ptr, m_buffer->size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		return true;
	}

	RHIWritePixelBuffer::RHIWritePixelBuffer(RHIBuffer::Usage usage)
		: RHIBuffer(RHIBuffer::PixelUnpackBuffer, usage)
		, m_texture(nullptr)
	{

	}

	bool RHIWritePixelBuffer::writeToTexture()
	{
		if (!m_texture)
		{
			return false;
		}
		auto ctx = getContext();
		ctx->setBuffer(this);
		ctx->setTexture2D(m_texture.get());
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_texture->getSize().width, m_texture->getSize().height,
			getGLReadFormat(m_texture->getPixelFormat()), GL_UNSIGNED_BYTE, nullptr);
		GL_CHECK();
		return true;
	}

	void RHIWritePixelBuffer::startWrite(std::vector<uint8> data)
	{
		if (m_texture)
		{
			getContext()->setBuffer(this);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
			v_check(ptr)
			if (ptr)
			{
				memcpy(ptr, data.data(), data.size());
			}
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			GL_CHECK()
		}
	}
}