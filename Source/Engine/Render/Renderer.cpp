#include "Render/Renderer.h"
#include <Core/Volucris.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application/Window.h>
#include <Core/TypesHelp.h>
#include <Application/FrameSynthesizer.h>
#include <Render/Command/RenderCommand.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIRenderTarget.h>
#include <RHI/RHIBuffer.h>
#include <Render/Frame.h>

namespace volucris
{
	Renderer::~Renderer()
	{
	}

	Renderer::Renderer()
		: Runable(1024)
	{
	}

	void Renderer::main()
	{
		m_frame->render(m_cmdList.get());
		FrameSynthesier::getInstance().countRenderFrame();
	}

	void Renderer::run()
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		m_window = std::make_unique<Window>();
		m_window->create(true);
		start(std::bind(&Renderer::main, this));
	}

	bool Renderer::initialize()
	{
		m_cmdList = std::make_unique<RHICommandList>();
		bool inited = m_cmdList->initialize(m_window.get());
		if (inited)
		{
			m_frame = std::make_unique<Frame>();
			m_frame->resize(800, 600);
		}
		return inited;
	}

	void Renderer::destroy()
	{
		m_frame = nullptr;
		m_window->destroy();
		m_cmdList->destroy();
	}
}
