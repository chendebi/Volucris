#include "RHI/RHIVertexArray.h"
#include <RHI/RHIOpenGL.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIVertexBuffer.h>

namespace volucris
{
	static GLenum getGLDataType(RHIVertexBuffer::DataType type)
	{
		switch (type)
		{
		case volucris::RHIVertexBuffer::Float:
			return GL_FLOAT;
		default:
			break;
		}
		return GL_NONE;
	}

	RHIVertexArray::RHIVertexArray(RHICommandList* context)
		: RHIResource(context)
		, m_buffer(nullptr)
		, m_id(0)
	{
	}

	RHIVertexArray::~RHIVertexArray()
	{
		if (m_id > 0)
		{
			glDeleteVertexArrays(1, &m_id);
			getContext()->unsetVertexArray(this);
		}
	}

	void RHIVertexArray::init(const std::shared_ptr<RHIVertexBuffer>& buffer)
	{
		m_buffer = buffer;
		glGenVertexArrays(1, &m_id);
		auto ctx = getContext();
		ctx->setVertexArray(this);
		ctx->setBuffer(buffer.get());

		for (const auto& desc : buffer->getDescriptions())
		{
			glEnableVertexAttribArray(desc.location);
			GLboolean normalized = desc.normalized ? GL_TRUE : GL_FALSE;
			auto type = getGLDataType(desc.type);
			glVertexAttribPointer(desc.location, desc.size, type, normalized, desc.stride, (void*)desc.offset);
		}

		GL_CHECK()
	}
}
