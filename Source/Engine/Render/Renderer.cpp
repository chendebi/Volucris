#include "Render/Renderer.h"
#include <Core/Volucris.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application/Window.h>
#include <Core/TypesHelp.h>
#include <Application/FrameSynthesizer.h>
#include <Render/Command/RenderCommand.h>
#include <RHI/RHICommandList.h>

namespace volucris
{
	static uint32 frameBuffer = 0;
	static uint32 texture = 0;

	Renderer::Renderer()
		: Runable(1024)
	{
	}

	void Renderer::main()
	{
		RENDER_SCOPE(BeginFrame)
		ENQUEUE_COMMMAND_LIST(ClearBuffer, [](RHICommandList* cmdList) {
			cmdList->setViewport(0, 0, 1920, 1080);
			cmdList->
			});
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
		return RHICmdList.initialize(std::move(m_window));
	}

	void Renderer::destroy()
	{
		RHICmdList.destroy();
	}
}
