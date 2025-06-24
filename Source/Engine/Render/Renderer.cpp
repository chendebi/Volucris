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

namespace volucris
{
	Renderer::Renderer()
		: Runable(1024)
	{
	}

	void Renderer::main()
	{
		RENDER_SCOPE(BeginFrame)
		ENQUEUE_COMMMAND_LIST(BindRenderTarget, [this](RHICommandList* cmdList) {
			cmdList->setRenderTarget(m_renderTarget.get());
			});
		ENQUEUE_COMMMAND_LIST(ClearBuffer, [](RHICommandList* cmdList) {
			cmdList->setViewport(0, 0, 1920, 1080);
			//  cmdList->clear();
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
		bool inited = RHICmdList.initialize(std::move(m_window));
		if (inited)
		{
			m_renderTarget = std::make_shared<RHIRenderTarget>(Size(800, 600));
			m_renderTarget->attachColor(RHITextureDesc(), 0);

			ENQUEUE_COMMMAND_LIST(CreateRenderTarget, [this](RHICommandList* cmdList) {
				m_renderTarget->init(cmdList);
				});
		}
		return inited;
	}

	void Renderer::destroy()
	{
		ENQUEUE_COMMMAND_LIST(CreateRenderTarget, [this](RHICommandList* cmdList) {
			cmdList->deleteResource(m_renderTarget.get());
			});
		RHICmdList.destroy();
	}
}
