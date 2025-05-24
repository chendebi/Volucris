#include "Engine/Renderer/OpenGL/ogl_render_buffer_object.h"
#include <Engine/Renderer/context.h>
#include "Core/volucris.h"
#include <Renderer/OpenGL/ogl_check.h>

namespace volucris
{
	RenderBufferObject::RenderBufferObject()
		: FrameBufferComponent(ComponentType::RENDER_BUFFER)
	{

	}
	RenderBufferObject::~RenderBufferObject()
	{
		release();
	}

	bool RenderBufferObject::create()
	{
		if (m_id == 0)
		{
			glGenRenderbuffers(1, &m_id);
		}
		return m_id > 0;
	}

	void RenderBufferObject::release()
	{
		if (m_id > 0)
		{
			glDeleteRenderbuffers(1, &m_id);
			m_id = 0;
		}
	}

	bool RenderBufferObject::initialize(Context* context)
	{
		if (m_valid)
		{
			return true;
		}

		if (m_id == 0 || !m_size.isValid())
		{
			return false;
		}

		context->bindRenderBuffer(this);
		glRenderbufferStorage(GL_RENDERBUFFER, m_format, m_size.width, m_size.height);
		m_valid = true;
		GL_CHECK();
		return true;
	}
}

