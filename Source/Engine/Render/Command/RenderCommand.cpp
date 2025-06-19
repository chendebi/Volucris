#include "Render/Command/RenderCommand.h"
#include <glad/glad.h>

namespace volucris
{
	struct ClearCommand::Impl
	{
		glm::vec4 color;
		GLbitfield buffers;
	};

	ClearCommand::ClearCommand(float r, float g, float b, float a, int32 buffers)
		: RenderCommand()
		, m_impl(new Impl)
	{
		m_impl->color = { r, g, b, a };
		m_impl->buffers = 0;
		if (buffers & Buffer::ColorBuffer)
		{
			m_impl->buffers |= GL_COLOR_BUFFER_BIT;
		}
		if (buffers & Buffer::DepthBuffer)
		{
			m_impl->buffers |= GL_DEPTH_BUFFER_BIT;
		}
	}

	ClearCommand::~ClearCommand()
	{
		delete m_impl;
	}

	void ClearCommand::execute()
	{
		glClear(m_impl->buffers);
	}
}
