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
#include <Render/View.h>
#include <Core/VectorHelp.h>
#include <Render/Scene.h>

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
		for (auto& view : m_views)
		{
			// todo: frame update
			view->render(m_cmdList.get());
		}
		FrameSynthesier::getInstance().countRenderFrame();
	}

	void Renderer::run()
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		auto context = glfwGetCurrentContext();
		glfwMakeContextCurrent(nullptr);
		m_window = std::make_unique<Window>();
		m_window->create(true);
		glfwMakeContextCurrent(context);
		start(std::bind(&Renderer::main, this));
	}

	bool Renderer::initialize()
	{
		m_cmdList = std::make_unique<RHICommandList>();
		bool inited = m_cmdList->initialize(m_window.get());
		return inited;
	}

	void Renderer::destroy()
	{
		m_views.clear();
		m_window->destroy();
		m_cmdList->destroy();
	}

	void Renderer::addView(std::unique_ptr<View> view)
	{
		m_views.emplace_back(std::move(view));
	}

	void Renderer::removeView(View* view)
	{
		VectorHelp::quickRemoveAllIf<std::unique_ptr<View>>(m_views, [view](const std::unique_ptr<View>& v) {
			return v.get() == view;
			});
	}
}
