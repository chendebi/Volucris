#include "Application/application.h"
#include <Core/assert.h>
#include "Core/volucris.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application/window.h"
#include "Renderer/context.h"

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
		, m_context(nullptr)
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
			setWindow(std::make_shared<Window>());
		}

		m_window->initialize();
		m_window->onClose.addObject(this, &Application::quit);

		m_context = std::make_shared<Context>();

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
			m_context->swapBuffers();
		}
	}
}
