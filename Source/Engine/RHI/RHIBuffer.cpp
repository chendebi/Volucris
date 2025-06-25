#include <RHI/RHIBuffer.h>
#include <glad/glad.h>
#include <Core/Volucris.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHITexture.h>
#include <RHI/RHIOpenGL.h>

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
		glBufferData(buffer.target, buffer.size, nullptr, buffer.usage);
		return true;
	}

	uint32 RHIBuffer::create(RHICommandList* command)
	{
		uint32 id;
		glGenBuffers(1, &id);
		return id;
	}

	void RHIBuffer::bind(RHIState* state)
	{
		glBindBuffer(m_impl->buffer.target, getId());
	}

	void RHIBuffer::destroy(RHICommandList* command)
	{
		auto id = getId();
		glDeleteBuffers(1, &id);
	}


	RHIReadPixelBuffer::RHIReadPixelBuffer(size_t size, EBufferUsage usage)
		: RHIBuffer(std::make_unique<Impl>(Impl::GLBuffer({size, GL_PIXEL_PACK_BUFFER, getGLUsage(usage)})))
	{
	}

	void RHIReadPixelBuffer::readColor(RHICommandList* command, Rect rect, RHIRenderTarget* renderTarget, int index)
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
		glReadPixels(rect.x, rect.y, rect.width, rect.height, getGLFormat(texture->getPixelFormat()), GL_UNSIGNED_BYTE, 0);
	}


}