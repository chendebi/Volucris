#include "Renderer/context.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Application/application.h"
#include "Application/window.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_vertex_array_object.h"
#include "Renderer/OpenGL/ogl_program_object.h"
#include "Core/volucris.h"
#include "Core/assert.h"
#include "Renderer/material_proxy.h"
#include "Renderer/primitive_proxy.h"

namespace volucris
{
	GLenum getDrawMode(const DrawMode& mode)
	{
		switch (mode)
		{
		case DrawMode::TRIANGLES:
			return GL_TRIANGLES;
		default:
			break;
		}
		check(false);
		return GL_NONE;
	}

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
		if (!vbo || m_renderState.vbo == vbo)
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

	void Context::bindElementBufferObjct(OGLBufferObject* ebo)
	{
		if (!ebo || m_renderState.drawState.ebo == ebo)
		{
			return;
		}

		auto target = ebo->getTarget();
		if (target != GL_ELEMENT_ARRAY_BUFFER)
		{
			return;
		}

		auto id = ebo->getID();
		if (id > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			m_renderState.drawState.ebo = ebo;
		}
	}

	void Context::bindVertexArrayObject(OGLVertexArrayObject* vao)
	{
		auto id = vao->getID();
		if (id > 0)
		{
			glBindVertexArray(id);
			m_renderState.drawState.vao = vao;
		}
	}

	void Context::setViewport(int x, int y, int w, int h)
	{
		setViewport({ x, y, w, h });
	}

	void Context::setViewport(const Rect& rect)
	{
		if (rect != m_viewport)
		{
			glViewport(rect.x, rect.y, rect.w, rect.h);
			m_viewport = rect;
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

	void Context::draw(const OGLDrawState& state, const SectionRenderData& section)
	{
		if (!prepareDrawState(state))
		{
			V_LOG_WARN(Engine, "draw call failed");
			return;
		}
		if (state.programState.program != m_renderState.drawState.programState.program)
		{
			m_renderState.drawState.programState.program = state.programState.program;
			glUseProgram(state.programState.program->getID());
			if (state.programState.uploader)
			{
				state.programState.uploader();
			}
		}

		bindVertexArrayObject(state.vao);
		bindElementBufferObjct(state.ebo);
		glDrawArrays(getDrawMode(section.mode), section.offset, section.count);
	}

	void Context::draw2(const MaterialProxy* material, const SectionDrawData& section)
	{
		OGLDrawState state;
		state.programState = material->getState();
		state.ebo = section.renderInfo->ebo.get();
		state.vao = section.renderInfo->vao.get();
		draw(state, *section.section);
	}

	bool Context::prepareDrawState(const OGLDrawState& state)
	{
		if (!state.ebo || !state.programState.program || !state.vao)
		{
			return false;
		}

		if (!state.vao->valid() && (!state.vao->create() || !state.vao->initialize(this)))
		{
			return false;
		}

		if (!state.ebo->valid() && (!state.ebo->create() || !state.ebo->initialize(this)))
		{
			return false;
		}

		if (!state.programState.program->valid() && !state.programState.program->initialize())
		{
			return false;
		}

		return true;
	}
}
