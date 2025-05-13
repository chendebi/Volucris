#include "Renderer/context.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Application/application.h"
#include "Application/window.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_vertex_array_object.h"

namespace volucris
{
	struct Context::Impl
	{
		GLFWwindow* window;
	};

	Context::Context()
		: m_impl(new Impl)
	{
		auto window = Application::Instance()->getWindow();
		m_impl->window = static_cast<GLFWwindow*>(window->getHandle());
		makeCurrent();
		glfwSwapInterval(1); // Enable vsync
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Context::~Context()
	{
		delete m_impl;
	}

	void Context::makeCurrent()
	{
		glfwMakeContextCurrent(m_impl->window);
	}

	void Context::swapBuffers()
	{
		glfwSwapBuffers(m_impl->window);
	}

	void Context::bindVertexBufferObject(OGLBufferObject* vbo)
	{
		if (m_renderState.vbo == vbo)
		{
			return;
		}

		auto target = vbo->getTarget();
		if (target != GL_ARRAY_BUFFER)
		{
			return;
		}

		auto id = vbo->getID();
		if (id > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, id);
			m_renderState.vbo = vbo;
		}
	}

	void Context::bindVertexArrayObject(OGLVertexArrayObject* vao)
	{
		auto id = vao->getID();
		if (id > 0)
		{
			glBindVertexArray(id);
			m_renderState.vao = vao;
		}
	}

	void Context::clear(const OGLClearState& state)
	{
		int flags = 0;
		if (state.clearBuffers & OGLClearState::COLOR_BUFFER)
		{
			if (state.color != m_clearState.color)
			{
				m_clearState.color = state.color;
				glClearColor(m_clearState.color.r, m_clearState.color.g, m_clearState.color.b, m_clearState.color.a);
			}
			flags |= GL_COLOR_BUFFER_BIT;
		}

		if (state.clearBuffers & OGLClearState::DEPTH_BUFFER)
		{
			if (state.depth != m_clearState.depth)
			{
				m_clearState.depth = state.depth;
				glClearDepth(m_clearState.depth);
			}
			flags |= GL_DEPTH_BUFFER_BIT;
		}

		if (flags)
		{
			glClear(flags);
		}
	}
}
