#include "Application/Window.h"
#include <Application/Application.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <Render/ImGuiRenderer.h>
#include <Core/Volucris.h>

namespace volucris
{
	Window::Window()
		: Widget()
		, m_handle(nullptr)
		, m_imguiRenderer(nullptr)
		, m_title("Window")
	{
	}

	Window::~Window()
	{
		
	}

	void Window::setTitle(const std::string& title)
	{
		m_title = title;
		if (m_handle)
		{
			glfwSetWindowTitle(m_handle, title.c_str());
		}
	}

	void Window::create(bool offscreen)
	{
		V_LOG_DEBUG(Engine, "create window: {}", m_title);
		m_handle = glfwCreateWindow(800, 600, m_title.c_str(), nullptr, nullptr);
		if (!offscreen)
		{
			glfwSetWindowUserPointer(m_handle, this);
			glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* handle) {
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
				gApp->removeWindow(window->getShared<Window>());
				});

			glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* handle, int width, int height) {
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
				window->setSize({ width, height });
				});

			m_imguiRenderer = std::make_unique<ImGuiRenderer>(m_handle);
		}
	}

	void Window::destroy()
	{
		V_LOG_DEBUG(Engine, "destroy window: {}", m_title);
		if (m_handle)
		{
			glfwDestroyWindow(m_handle);
			m_handle = nullptr;
		}
	}

	void Window::destroyImGuiRenderer()
	{
		m_imguiRenderer = nullptr;
	}

	void Window::onBuild()
	{
		m_imguiRenderer->makeCurrent();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
}
