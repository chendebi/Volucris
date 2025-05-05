#include "Application/application.h"
#include <Core/assert.h>
#include "Core/volucris.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application/window.h"
#include "Application/widget.h"
#include "Renderer/context.h"
#include "Renderer/renderer.h"

namespace volucris
{
	Application* Application::Inst = nullptr;

	Application::Config::Config(int argc, char* argv[])
		: appName("volucris")
		, arguments()
	{
		arguments.reserve(argc);
		for (auto idx = 0; idx < argc; ++idx)
		{
			arguments.push_back(argv[idx]);
		}
	}

	Application::Application(const Config& config)
		: m_config(config)
		, m_initialized(0)
		, m_running(0)
		, m_window(nullptr)
		, m_renderer(nullptr)
		, m_mainWidget(nullptr)
	{
		checkq(!Inst, Engine, "application already exist");
		Inst = this;
	}

	bool Application::initialize()
	{
		if (m_initialized)
		{
			return true;
		}

		if (glfwInit() != GLFW_TRUE)
		{
			return false;
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		if (!m_window)
		{
			auto window = std::make_shared<Window>();
			window->setTitle(m_config.appName);
			window->setSize(1200, 800);
			setWindow(window);
		}

		m_window->initialize();
		m_window->onClose.addObject(this, &Application::quit);

		if (!m_renderer)
		{
			m_renderer = std::make_shared<Renderer>();
		}

		if (!m_renderer->getContext())
		{
			m_renderer->setContext(std::make_shared<Context>());
		}
		
		if (!m_window->setupImGUI("#version 330"))
		{
			return false;
		}

		m_initialized = true;
		return true;
	}

	void Application::shotdown()
	{
		if (!m_initialized)
		{
			return;
		}
		m_window->destroy();
		glfwTerminate();
		m_initialized = false;
	}

	bool Application::setWindow(const std::shared_ptr<Window>& window)
	{
		if (m_initialized)
		{
			return false;
		}

		m_window = window;
		return true;
	}

	bool Application::setRenderer(const std::shared_ptr<Renderer>& renderer)
	{
		if (m_running)
		{
			return false;
		}
		m_renderer = renderer;
		return true;
	}

	void Application::setMainWidget(const std::shared_ptr<Widget>& widget)
	{
		m_mainWidget = widget;
	}

	Context* Application::getContext() const
	{
		if (m_renderer)
		{
			return m_renderer->getContext();
		}
		return nullptr;
	}

	void Application::quit()
	{
		V_LOG_INFO(Engine, "quit application");
		m_running = false;
	}

	void Application::exec()
	{
		if (m_running || !m_initialized)
		{
			return;
		}

		m_running = true;

		while (m_running)
		{
			m_window->pollEvents();

			Widget::draw(m_mainWidget.get());

			m_renderer->render();
		}
	}
}
