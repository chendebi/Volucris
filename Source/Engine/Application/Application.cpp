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
		: m_focusedWindow(nullptr)
		, m_mainWindow(nullptr)
		, m_windows()
		, m_queue(1024)
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

		setFocusedWindow(window.get());
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
			setFocusedWindow(nullptr);
		}
		else
		{
			window->destroyImGuiRenderer();
			// 上下文切换到主窗口
			setFocusedWindow(m_mainWindow.get());

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

	void Application::setFocusedWindow(Window* window)
	{
		m_focusedWindow = window;
		if (m_focusedWindow && m_focusedWindow->getImGuiRenderer())
		{
			m_focusedWindow->getImGuiRenderer()->makeCurrent();

			// 渲染一帧
			m_focusedWindow->build();

			m_focusedWindow->getImGuiRenderer()->render();
		}
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

			m_focusedWindow->build();

			/*for (auto renderer : m_renderers)
			{
				for (auto scene : m_scenes)
				{
					scene->update(renderer);
				}
			}*/

			Renderer::getInstance().push(nullptr);

			FrameSynthesier::getInstance().countGameFrame();

			m_focusedWindow->getImGuiRenderer()->render();

			glfwPollEvents();
		}

		Renderer::getInstance().quit();
		return 0;
	}
}
