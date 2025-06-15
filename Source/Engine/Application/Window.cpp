#include "Application/Window.h"
#include <Application/Application.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <Render/ImGuiRenderer.h>

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

	void Window::create()
	{
		m_handle = glfwCreateWindow(800, 600, m_title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_handle, this);
		glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* handle) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
			gApp->removeWindow(window->getShared<Window>());
			});

		m_imguiRenderer = std::make_unique<ImGuiRenderer>(m_handle);
	}

	void Window::destroy()
	{
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
