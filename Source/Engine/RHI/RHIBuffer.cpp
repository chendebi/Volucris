#include <RHI/RHIBuffer.h>
#include <glad/glad.h>
#include <Core/Volucris.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHITexture.h>
#include <RHI/RHIOpenGL.h>
#include <FileSystem/FileSystem.h>

namespace volucris
{
	static GLenum getGLUsage(RHIBuffer::EBufferUsage usage)
	{
		switch (usage)
		{
		case volucris::RHIBuffer::StaticDraw:
			return GL_STATIC_DRAW;
		case volucris::RHIBuffer::DynamicDraw:
			return GL_DYNAMIC_DRAW;
		case volucris::RHIBuffer::StreamRead:
			return GL_STREAM_READ;
		case volucris::RHIBuffer::StreamWrite:
			return GL_STREAM_DRAW;
		default:
			break;
		}
		return GL_STATIC_DRAW;
	}

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

	static GLenum getGLPixelBufferTarget(RHIBuffer::EBufferUsage usage)
	{
		switch (usage)
		{
		case volucris::RHIBuffer::StreamRead:
			return GL_PIXEL_PACK_BUFFER;
		case volucris::RHIBuffer::StreamWrite:
			return GL_PIXEL_UNPACK_BUFFER;
		default:
			break;
		}
		V_LOG_WARN(Engine, "try create a pixel buffer object in error usage: {}", (int)usage)
		return GL_PIXEL_PACK_BUFFER;
	}

	struct RHIBuffer::Impl
	{
		struct GLBuffer
		{
			size_t size;
			GLenum target;
			GLenum usage;
		};
		GLBuffer buffer;

		Impl(GLBuffer buff)
			: buffer(buff)
		{ }
	};

	RHIBuffer::RHIBuffer(std::unique_ptr<Impl> buffer)
		: m_impl(std::move(buffer))
	{
	}

	bool RHIBuffer::init(RHICommandList* command)
	{
		const auto buffer = m_impl->buffer;
		command->bindResource(this);
		GL_CHECK();
		glBufferData(buffer.target, buffer.size, nullptr, buffer.usage);
		GL_CHECK();
		return true;
	}

	uint32 RHIBuffer::create(RHIState* state)
	{
		uint32 id;
		glGenBuffers(1, &id);
		return id;
	}

	void RHIBuffer::bind(RHIState* state)
	{
		glBindBuffer(m_impl->buffer.target, getId());
	}

	void RHIBuffer::destroy(RHIState* state)
	{
		auto id = getId();
		glDeleteBuffers(1, &id);
	}


	RHIReadPixelBuffer::RHIReadPixelBuffer(size_t size, EBufferUsage usage)
		: RHIBuffer(std::make_unique<Impl>(Impl::GLBuffer({size, GL_PIXEL_PACK_BUFFER, getGLUsage(usage)})))
	{
	}

	void RHIReadPixelBuffer::startRead(RHICommandList* command, Rect rect, RHIRenderTarget* renderTarget, int index)
	{
		auto target = renderTarget->getAttachedColor(index);
		if (!target)
		{
			return;
		}

		auto texture = dynamic_cast<RHITexture2D*>(target);
		if (!texture)
		{
			return;
		}

		command->bindResource(renderTarget);
		command->bindResource(this);
		GL_CHECK();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		GL_CHECK();
		glReadPixels(rect.x, rect.y, rect.width, rect.height, getGLReadFormat(texture->getPixelFormat()), GL_UNSIGNED_BYTE, 0);
		GL_CHECK();
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
	}

	std::vector<uint8> RHIReadPixelBuffer::readColor(RHICommandList* command)
	{
		command->bindResource(this);
		void* ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if (ptr == nullptr)
		{
			V_LOG_ERROR(Engine, "Failed to map pixel buffer for reading color data.")
			return {};
		}
		std::vector<uint8> colorData(m_impl->buffer.size);
		memcpy(colorData.data(), ptr, m_impl->buffer.size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		return colorData;
	}

	bool RHIWritePixelBuffer::writeTo(RHITexture2D* texture, RHICommandList* command)
	{
		command->bindResource(this);
		command->bindResource(texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->getSize().width, texture->getSize().height,
			getGLReadFormat(texture->getPixelFormat()), GL_UNSIGNED_BYTE, nullptr);
		GL_CHECK();
		return true;
	}

	bool RHIReadPixelBuffer::readColorTo(std::vector<uint8>& data, RHICommandList* command)
	{
		command->bindResource(this);
		void* ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if (ptr == nullptr)
		{
			V_LOG_ERROR(Engine, "Failed to map pixel buffer for reading color data.")
			return false;
		}

		if (data.size() != m_impl->buffer.size)
		{
			return false;
		}
		memcpy(data.data(), ptr, m_impl->buffer.size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

		return true;
	}

	RHIWritePixelBuffer::RHIWritePixelBuffer(size_t size, EBufferUsage usage)
		: RHIBuffer(std::make_unique<Impl>(Impl::GLBuffer({ size, GL_PIXEL_UNPACK_BUFFER, getGLUsage(usage) })))
	{

	}

	void RHIWritePixelBuffer::startWrite(RHICommandList* command, std::vector<uint8> data)
	{
		command->bindResource(this);
		void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		if (ptr)
		{
			memcpy(ptr, data.data(), data.size());
		}
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}
}