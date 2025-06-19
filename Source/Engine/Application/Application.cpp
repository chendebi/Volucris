#include "Application/Application.h"
#include <stdexcept>
#include <Application/window.h>
#include <GLFW/glfw3.h>
#include <Core/VectorHelp.h>
#include <Render/ImGuiRenderer.h>
#include <Render/Renderer.h>
#include <Application/FrameSynthesizer.h>
#include <Core/Volucris.h>
#include <Engine/Profile/Profile.h>
#include <Profile/ProfileManager.h>
#include <tracy/Tracy.hpp>

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
			for (const auto& window : m_windows)
			{
				window->destroyImGuiRenderer();
			}

			for (const auto& window : m_windows)
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

		Renderer::getInstance().run();
		double lastFrameTime = glfwGetTime();
		while (m_mainWindow->isValid())
		{
			V_SCOPED_PROFILE;
			
			glfwPollEvents();

			for (const auto& window : m_windows)
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

			Renderer::getInstance().push(nullptr);

			FrameSynthesier::getInstance().countGameFrame();

			for (const auto& window : m_windows)
			{
				window->getImGuiRenderer()->render();
			}
			
		}

		Renderer::getInstance().quit();
		return 0;
	}
}
