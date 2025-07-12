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
		, m_id(0)
		, m_type(type)
		, m_usage(usage)
		, m_buffer(std::make_unique<RHIOpenGLBuffer>(RHIOpenGLBuffer({getGLTarget(type), getGLUsage(usage), 0})))
	{
	}

	RHIBuffer::~RHIBuffer()
	{	
	}

	void RHIBuffer::createGpuResource()
	{
		if (m_id == 0)
		{
			glGenBuffers(1, &m_id);
		}
	}

	void RHIBuffer::init(uint64 bufferSize)
	{
		GL_CHECK()
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