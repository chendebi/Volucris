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

		{
			const auto vender = glGetString(GL_VENDOR);
			const auto renderer = glGetString(GL_RENDERER);
			const auto language = glGetString(GL_SHADING_LANGUAGE_VERSION);
			const auto version = glGetString(GL_VERSION);

			V_LOG_INFO(Engine, "context initialized");
			V_LOG_INFO(Engine, "	vender: {}", (char*)vender);
			V_LOG_INFO(Engine, "	renderer: {}", (char*)renderer);
			V_LOG_INFO(Engine, "	version: {}", (char*)version);
			V_LOG_INFO(Engine, "	language version: {}", (char*)language);
		}
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

	void Context::bindUniformBlock(UniformBlock* block, uint32 index)
	{
		//GLint alignment;
		//glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
		//if (block->block.offset % alignment != 0) {
		//	V_LOG_ERROR(Engine, "block offset is invalid: {}, {}", block->block.offset, alignment);
		//}
		glBindBufferRange(GL_UNIFORM_BUFFER, index, block->ubo->getID(), block->block.offset, block->block.size);
		GL_CHECK();
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

	void Context::setCameraInfoBlock(UniformBlock* block)
	{
		m_cameraInfoBlock = block;
	}

	void Context::setDirectionLightBlock(UniformBlock* block)
	{
		m_directonLightBlock = block;
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
		glEnable(GL_DEPTH_TEST);
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

		if (state.programState.material != m_renderState.drawState.programState.material)
		{
			m_renderState.drawState.programState.material = state.programState.material;
			glUseProgram(m_renderState.drawState.programState.material->getProgramObject()->getID());
		}

		for (const auto& uniform : state.programState.uniforms)
		{
			uniform->upload();
		}

		
		GL_CHECK();
		bindVertexArrayObject(state.vao);
		bindBuffer(state.ebo);
		glDrawElements(getDrawMode(section.mode), section.count, GL_UNSIGNED_INT, (void*)section.offset);
		GL_CHECK();
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
		if (!state.ebo || !state.programState.material || !state.vao)
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

		if (!state.programState.material->ready())
		{
			return false;
		}

		GL_CHECK();
		return true;
	}

	bool Context::prepareUniformBlock(UniformBlock* block)
	{
		if (!block)
		{
			return false;
		}
		auto ubo = block->ubo;
		if (!ubo || (!ubo->valid() && (!ubo->create() || !ubo->initialize(this))))
		{
			return false;
		}
		return true;
	}
}
