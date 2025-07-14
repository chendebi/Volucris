#include <RHI/RHIBuffer.h>
#include <glad/glad.h>
#include <Core/Volucris.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHITexture.h>
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIOpenGLBuffer.h>
#include <thread>
#include <sstream>

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
		if (m_id > 0)
		{
			auto id = std::this_thread::get_id();
			std::ostringstream oss;
			oss << id;
			V_LOG_DEBUG(Engine, "delete buffer: {}, {}, {}", (int)m_type, m_id, oss.str());
			getContext()->unsetBuffer(this);
			glDeleteBuffers(1, &m_id);
		}
	}

	void RHIBuffer::createGpuResource()
	{
		if (m_id == 0)
		{
			glGenBuffers(1, &m_id);
			auto id = std::this_thread::get_id();
			std::ostringstream oss;
			oss << id;
			V_LOG_DEBUG(Engine, "create buffer: {}, {}, {}", (int)m_type, m_id, oss.str());
		}
	}

	void RHIBuffer::init(const std::vector<uint8>& data)
	{
		init(data.data(), data.size());
	}

	void RHIBuffer::init(const uint8* data, uint32 size)
	{
		m_buffer->size = size;
		glBufferData(m_buffer->target, size, data, m_buffer->usage);
		GL_CHECK()
	}
}