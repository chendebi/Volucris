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

namespace volucris
{
	Renderer::Renderer()
		: Runable(1024)
	{
	}

	void Renderer::main()
	{
		RENDER_SCOPE(Frame)
		ENQUEUE_COMMMAND_LIST(BindRenderTarget, [this](RHICommandList* cmdList) {
			cmdList->setRenderTarget(m_renderTarget.get());
			RHIClearState state;
			state.color = glm::vec4(1.0,0.0,1.0,1.0);
			cmdList->clear(state);
			m_reader->readColor(cmdList, {0, 0, 800, 600}, m_renderTarget.get());
			});
		FrameSynthesier::getInstance().countRenderFrame();
	}

	void Renderer::run()
	{
		glfwMakeContextCurrent(nullptr);
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
			RHITextureDesc desc;
			desc.pixelFormat = Texture::EPixelFormat::R8G8B8;
			m_renderTarget = std::make_shared<RHIRenderTarget>(Size(800, 600));
			m_renderTarget->attachColor(desc, 0);

			m_reader = std::make_shared<RHIReadPixelBuffer>(800 * 600 * 4, RHIBuffer::StreamRead);

			ENQUEUE_COMMMAND_LIST(CreateRenderTarget, [this](RHICommandList* cmdList) {
				m_renderTarget->init(cmdList);
				m_reader->init(cmdList);
				});
		}
		return inited;
	}

	void Renderer::destroy()
	{
		ENQUEUE_COMMMAND_LIST(CreateRenderTarget, [this](RHICommandList* cmdList) {
			cmdList->deleteResource(m_renderTarget.get());
			cmdList->deleteResource(m_reader.get());
			});
		RHICmdList.destroy();
	}
}
