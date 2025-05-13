#include "Renderer/OpenGL/ogl_vertex_array_object.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/context.h"
#include "Renderer/OpenGL/ogl_check.h"

namespace volucris
{
	OGLVertexArrayObject::OGLVertexArrayObject()
		: m_id(0)
		, m_valid(false)
		, m_vbo(nullptr)
		, m_descriptions()
	{

	}

	OGLVertexArrayObject::~OGLVertexArrayObject()
	{
		release();
	}

	bool OGLVertexArrayObject::create()
	{
		if (m_id > 0)
		{
			return true;
		}
		glGenVertexArrays(1, &m_id);
		return m_id > 0;
	}

	void OGLVertexArrayObject::release()
	{
		if (m_id > 0)
		{
			glDeleteVertexArrays(1, &m_id);
			m_id = 0;
		}
	}

	bool OGLVertexArrayObject::initialize(Context* ctx)
	{
		if (m_id == 0)
		{
			V_LOG_WARN(Engine, "try initialize invalid vao");
			return false;
		}

		if (!m_vbo || m_descriptions.empty())
		{
			return false;
		}

		if (!m_vbo->valid())
		{
			m_vbo->create();
			if (!m_vbo->initialize(ctx))
			{
				return false;
			}
		}

		ctx->bindVertexBufferObject(m_vbo.get());
		ctx->bindVertexArrayObject(this);
		
		auto idx = 0;
		for (const auto& desc : m_descriptions)
		{
			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx, desc.size, desc.type, desc.normalized ? GL_TRUE : GL_FALSE, desc.stride, (void*)desc.offset);
			++idx;
		}
		GL_CHECK();
		m_valid = true;
		return true;
	}
}
