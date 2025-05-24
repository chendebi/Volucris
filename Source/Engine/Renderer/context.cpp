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
#include "Renderer/OpenGL/ogl_uniform.h"
#include <Renderer/OpenGL/ogl_check.h>
#include "Renderer/mesh_proxy.h"
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Renderer/OpenGL/ogl_render_buffer_object.h>
#include <Renderer/OpenGL/ogl_frame_buffer_object.h>

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

	void Context::bindBuffer(OGLBufferObject* buffer)
	{
		if (!buffer || buffer->getID() == 0)
		{
			return;
		}
		auto target = buffer->getTarget();

		if (target == GL_ARRAY_BUFFER && m_renderState.vbo == buffer) return;
		else if (target == GL_ELEMENT_ARRAY_BUFFER && m_renderState.drawState.ebo == buffer) return;
		else if (target == GL_UNIFORM_BUFFER && m_renderState.ubo == buffer) return;
		glBindBuffer(target, buffer->getID());
	}

	void Context::bindUniformBuffer(OGLBufferObject* buffer, uint32 index)
	{
		if (!buffer || buffer->getTarget()!=GL_UNIFORM_BUFFER || buffer->getID() == 0)
		{
			return;
		}

		auto it = m_renderState.ubos.find(index);
		if (it != m_renderState.ubos.end())
		{
			if (m_renderState.ubo == buffer)
			{
				return;
			}
			it->second = buffer;
		}
		else
		{
			m_renderState.ubos[index] = buffer;
		}
		m_renderState.ubo = buffer;
		glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer->getID());
	}

	void Context::bindUniformBlock(const UniformBlock& block, uint32 index)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, index, block.ubo->getID(), block.block.offset, block.block.size);
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

	void Context::bindTexture2D(Texture2DObject* texture2d)
	{
		auto id = texture2d->getID();
		if (id > 0 && m_renderState.texture2d != texture2d)
		{
			m_renderState.texture2d = texture2d;
			glBindTexture(GL_TEXTURE_2D, id);
		}
	}

	void Context::bindRenderBuffer(RenderBufferObject* rbo)
	{
		auto id = rbo->getID();
		if (id > 0 && rbo != m_renderState.rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			m_renderState.rbo = rbo;
		}
	}

	void Context::bindFrameBuffer(FrameBufferObject* fbo)
	{
		if (fbo)
		{
			auto id = fbo->getID();
			if (id > 0 && fbo != m_renderState.fbo)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, id);
				m_renderState.fbo = fbo;
			}
		}
		else
		{
			m_renderState.fbo = nullptr;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void Context::setCameraInfoBlock(const UniformBlock& block)
	{
		m_cameraInfoBlock = block;
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

	bool Context::beginRenderPass(FrameBufferObject* target)
	{
		if (target)
		{
			if (!target->create() || !target->initialize(this))
			{
				return false;
			}
			target->updateAttachments(this);
			bindFrameBuffer(target);
		}
		return true;
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
		}

		for (const auto& uniform : state.programState.uniforms)
		{
			uniform->upload();
		}

		bindVertexArrayObject(state.vao);
		bindBuffer(state.ebo);
		glDrawElements(getDrawMode(section.mode), section.count, GL_UNSIGNED_INT, (void*)section.offset);
	}

	void Context::draw(const MaterialProxy* material, const SectionDrawData& section)
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

		if (!state.ebo->valid() && 
			(state.ebo->getTarget() != GL_ELEMENT_ARRAY_BUFFER 
				|| !state.ebo->create() 
				|| !state.ebo->initialize(this))
		   )
		{
			return false;
		}

		if (!state.programState.program->valid() && !state.programState.program->initialize())
		{
			return false;
		}

		uint32 slot = 0;
		for (const auto& uniformBlock : state.programState.program->getUniformBlockDescriptions())
		{
			glUniformBlockBinding(state.programState.program->getID(), uniformBlock.location, slot);
			switch (uniformBlock.desc.type)
			{
			case MaterialParameterDesc::CAMERA_INFO:
				if (!prepareUniformBlock(m_cameraInfoBlock))
				{
					return false;
				}
				bindUniformBlock(m_cameraInfoBlock, slot);
				break;
			default:
				break;
			}
			++slot;
		}

		return true;
	}

	bool Context::prepareUniformBlock(const UniformBlock& block)
	{
		auto ubo = m_cameraInfoBlock.ubo;
		if (!ubo || (!ubo->valid() && (!ubo->create() || !ubo->initialize(this))))
		{
			return false;
		}
		return true;
	}
}
