#include "Application/Application.h"
#include <stdexcept>
#include <Application/window.h>
#include <GLFW/glfw3.h>
#include <Core/VectorHelp.h>
#include <Render/ImGuiRenderer.h>


namespace volucris
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance != nullptr)
		{
			throw std::runtime_error("Application instance already exists.");
		}
		s_instance = this;
	}

	void Application::addWindow(const std::shared_ptr<Window>& window)
	{
		if (window->isValid())
		{
			return;
		}

		window->create();
		m_windows.push_back(window);
		if (m_mainWindow == nullptr)
		{
			m_mainWindow = window;
		}

		// todo 添加渲染线程
	}

	void Application::removeWindow(const std::shared_ptr<Window>& window)
	{
		if (!window || !window->isValid())
		{
			return;
		}

		if (window == m_mainWindow)
		{
			for (auto window : m_windows)
			{
				window->destroyImGuiRenderer();
			}

			for (auto window : m_windows)
			{
				window->destroy();
			}
			m_windows.clear();
		}
		else
		{
			window->destroyImGuiRenderer();
			// 上下文切换到主窗口
			m_mainWindow->getImGuiRenderer()->makeCurrent();
			window->destroy();
			VectorHelp::quickRemove(m_windows, window);
		}
	}

	void Application::setMainWindow(const std::shared_ptr<Window>& window)
	{
		if (!window->isValid())
		{
			addWindow(window);
		}
		m_mainWindow = window;
	}

	int Application::exec()
	{
		if (!m_mainWindow)
		{
			return 0;
		}

		while (m_mainWindow->isValid())
		{
			glfwPollEvents();

			for (auto window : m_windows)
			{
				window->build();
			}

			/*for (auto renderer : m_renderers)
			{
				for (auto scene : m_scenes)
				{
					scene->update(renderer);
				}
			}*/

			for (auto window : m_windows)
			{
				window->getImGuiRenderer()->render();
			}
		}

		return 0;
	}
}
