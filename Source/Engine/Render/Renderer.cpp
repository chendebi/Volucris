#include "Render/Renderer.h"
#include <Core/Volucris.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application/Window.h>
#include <Core/TypesHelp.h>
#include <Application/FrameSynthesizer.h>

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
		glClear(GL_COLOR_BUFFER_BIT);
		FrameSynthesier::getInstance().countRenderFrame();
	}

	void Renderer::run()
	{
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		m_window = std::make_unique<Window>();
		m_window->create(true);
		start(std::bind(&Renderer::main, this));
	}

	bool Renderer::initialize()
	{
		auto handle = m_window->getHandle();
		glfwMakeContextCurrent(handle);
		glfwSwapInterval(0);
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

		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1080, 960, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		glViewport(0, 0, 1080, 960);
		glClearColor(0.2, 0.4, 0.8, 1.0);
		return true;
	}

	void Renderer::destroy()
	{
		m_window->destroy();
		m_window = nullptr;
	}
}
