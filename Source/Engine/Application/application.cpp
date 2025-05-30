#include "Application/application.h"
#include <Core/assert.h>
#include "Core/volucris.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application/window.h"
#include "Application/widget.h"
#include "Renderer/context.h"
#include "Renderer/renderer.h"
#include "Scene/level.h"

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
		, m_delta(0)
		, m_window(nullptr)
		, m_renderer(nullptr)
		, m_mainWidget(nullptr)
		, m_level(nullptr)
		, m_queue(1024)
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
			getWindow();
		}

		m_window->initialize();
		m_window->Close.addObject(this, &Application::quit);

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

		m_window->FrameSizeChanged.addObject(m_renderer.get(), & Renderer::setWindowFrameSize);
		m_window->MouseMove.addLambda([this](int x, int y) {
			m_level->dispatchMouseMoveEvent(x, y);
			});
		m_window->MousePressed.addLambda([this](const MouseEvent& e) {
			m_level->dispatchMousePressEvent(e);
			});
		m_window->MouseReleased.addLambda([this](const MouseEvent& e) {
			m_level->dispatchMouseReleaseEvent(e);
			});

		m_initialized = true;
		return true;
	}

	void Application::shutdown()
	{
		if (!m_initialized)
		{
			return;
		}

		m_window->FrameSizeChanged.removeAll(m_renderer.get());
		m_window->destroy();
		glfwTerminate();
		m_initialized = false;
		V_LOG_INFO(Engine, "application quit success");
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

	void Application::setLevel(const std::shared_ptr<Level>& level)
	{
		if (m_level)
		{
			m_level->deattachFromRenderer();
		}
		m_level = level;
		m_level->attachToRenderer();
		CurrentLevelChanged(m_level);
	}

	Window* Application::getWindow()
	{
		if (!m_window)
		{
			m_window = std::make_shared<Window>();
			m_window->setTitle(m_config.appName);
			m_window->setSize(1200, 800);
		}
		return m_window.get();
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

		if (!m_level)
		{
			setLevel(std::make_shared<Level>());
		}

		std::function<void()> command;
		auto time = glfwGetTime();
		while (m_running)
		{
			while (m_queue.pop(command))
			{
				command();
			}

			m_window->pollEvents();

			Widget::draw(m_mainWidget.get());

			// TODO: tick
			tick(m_delta);

			m_level->tick(m_delta);

			m_level->update();

			m_renderer->render();

			auto now = glfwGetTime();
			m_delta = now - time;
			time = now;
		}

		while (m_queue.pop(command))
		{
			command();
		}

		m_level->deattachFromRenderer();

		m_renderer->render();
		m_renderer->clearCommands();

		shutdown();
	}

	void Application::pushCommand(const std::function<void()>& command)
	{
		m_queue.push(command);
	}

	StatInfo Application::getStatInfo() const
	{
		StatInfo info;
		info.frameTime = m_delta;
		info.frameRate = 1.0 / m_delta;
		return info;
	}
}
