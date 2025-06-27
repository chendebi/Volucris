#include <RHI/RHICommandList.h>
#include <Application/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Volucris.h>
#include <RHI/RHITexture.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIRenderTarget.h>

namespace volucris
{
	struct RHICommandList::Impl
	{
		GLbitfield clearFlags;
	};

	GLbitfield getGLClearFlags(uint32 buffers)
	{
		GLbitfield flags = 0;
		if (buffers & RHIClearState::ColorBuffer)
		{
			flags |= GL_COLOR_BUFFER_BIT;
		}
		if (buffers & RHIClearState::DepthBuffer)
		{
			flags |= GL_DEPTH_BUFFER_BIT;
		}

		return flags;
	}

	RenderScope::RenderScope(const std::string& name)
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name.c_str());
	}

	RenderScope::~RenderScope()
	{
		glPopDebugGroup();
	}

	RHICommandList::RHICommandList()
		: m_window(nullptr)
		, m_impl(new Impl)
	{
		m_impl->clearFlags = getGLClearFlags(m_state.clearState.buffers);
	}

	RHICommandList::~RHICommandList()
	{
		delete m_impl;
	}

	bool RHICommandList::initialize(Window* window, bool sync)
	{
		m_window = window;
		makeCurrent();
		glfwSwapInterval(sync ? 1 : 0);
		gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

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
		return true;
	}

	void RHICommandList::destroy()
	{
		m_window = nullptr;
	}

	void RHICommandList::makeCurrent()
	{
		glfwMakeContextCurrent(m_window->getHandle());
	}

	void RHICommandList::swapBuffers()
	{
		glfwSwapBuffers(m_window->getHandle());
	}

	void RHICommandList::clear(const RHIClearState& state)
	{
		if (state.color != m_state.clearState.color)
		{
			m_state.clearState.color = state.color;
			glClearColor(state.color.r, state.color.g, state.color.b, state.color.a);
		}

		if (state.buffers != m_state.clearState.buffers)
		{
			m_impl->clearFlags = getGLClearFlags(m_state.clearState.buffers);
			m_state.clearState.buffers = state.buffers;
		}
		glClear(m_impl->clearFlags);
	}

	void RHICommandList::executeCommand(const std::string& name)
	{
		//glDebug
	}

	void RHICommandList::setTexture(RHITexture* texture)
	{
		bindResource(texture);
	}

	void RHICommandList::setRenderTarget(RHIRenderTarget* renderTarget)
	{
		bindResource(renderTarget);
	}

	void RHICommandList::bindResource(RHIResource* resource)
	{
		if (!resource->isCreated())
		{
			auto id = resource->create(this);
			resource->m_id = id;
		}
		resource->bind(&m_state);
	}

	void RHICommandList::deleteResource(RHIResource* resource)
	{
		if (resource)
		{
			resource->destroy(this);
			resource->m_id = 0;
		}
	}

	void RHICommandList::setViewport(int x, int y, int w, int h)
	{
		Rect viewport = { x, y, w, h };
		if (m_state.viewport != viewport)
		{
			glViewport(x, y, w, h);
			m_state.viewport = viewport;
		}
	}

}