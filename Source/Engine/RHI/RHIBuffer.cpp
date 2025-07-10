#include <RHI/RHIBuffer.h>
#include <glad/glad.h>
#include <Core/Volucris.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHITexture.h>
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIOpenGLBuffer.h>

namespace volucris
{
	static GLenum getGLTarget(RHIBuffer::Type type)
	{
		switch (type)
		{
		case volucris::RHIBuffer::VertexBuffer:
			return GL_VERTEX_ARRAY;
		case volucris::RHIBuffer::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		case volucris::RHIBuffer::PixelPackBuffer:
			return GL_PIXEL_PACK_BUFFER;
		case volucris::RHIBuffer::PixelUnpackBuffer:
			return GL_PIXEL_UNPACK_BUFFER;
		default:
			break;
		}
		return GL_NONE;
	}

	static GLenum getGLUsage(RHIBuffer::Usage usage)
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


	RHIBuffer::RHIBuffer(Type type, Usage usage)
		: RHIResource()
		, m_type(type)
		, m_usage(usage)
		, m_buffer(std::make_unique<RHIOpenGLBuffer>(RHIOpenGLBuffer({getGLTarget(type), getGLUsage(usage), 0})))
	{
	}

	RHIBuffer::~RHIBuffer()
	{
		auto id = getId();
		if (id > 0)
		{
			glDeleteBuffers(1, &id);
		}
	}

	void RHIBuffer::init(uint64 bufferSize)
	{
		glBufferData(m_buffer->target, bufferSize, nullptr, m_buffer->usage);
		m_buffer->size = bufferSize;
		GL_CHECK()
	}

	void RHIBuffer::init(const std::vector<uint8>& data)
	{
		m_buffer->size = data.size();
		glBufferData(m_buffer->target, m_buffer->size, data.data(), m_buffer->usage);
		GL_CHECK()
	}
}