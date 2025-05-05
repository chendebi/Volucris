#include "Application/window.h"
#include <Core/types_help.h>
#include <GLFW/glfw3.h>
#include "Application/application.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace volucris
{
	struct Window::Impl
	{
		uint8 imguiInited = false;
		GLFWwindow* handle = nullptr;
	};

	void onGLFWWindowClose(GLFWwindow* handle)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		window->onClose();
	}

	Window::Window()
		: m_impl(new Impl)
		, m_width(800)
		, m_height(600)
	{

	}

	Window::~Window()
	{
		destroy();
		delete m_impl;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	void Window::setTitle(const std::string& title)
	{
		m_title = title;
	}

	void Window::setSize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	bool Window::initialize()
	{
		if (m_impl->handle)
		{
			return true;
		}

		if (!Application::Instance() || Application::Instance()->getWindow() != this
			|| Application::Instance()->isInitialized())
		{
			return false;
		}

		m_impl->handle = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_impl->handle, this);
		glfwSetWindowCloseCallback(m_impl->handle, onGLFWWindowClose);
		return true;
	}

	void Window::destroy()
	{
		if (m_impl->handle && !Application::Instance()->isRunning())
		{
			if (m_impl->imguiInited)
			{
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
				m_impl->imguiInited = false;
			}

			glfwDestroyWindow(m_impl->handle);
			m_impl->handle = nullptr;
		}
	}

	void* Window::getHandle() const
	{
		return m_impl->handle;
	}

	bool Window::setupImGUI(const std::string& glslversion)
	{
		if (m_impl->imguiInited)
		{
			return true;
		}

		auto app = Application::Instance();
		if (!m_impl->handle || app->isRunning() && !app->getContext())
		{
			return false;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_impl->handle, true);
		ImGui_ImplOpenGL3_Init(glslversion.c_str());
		m_impl->imguiInited = true;
		return true;
	}
}