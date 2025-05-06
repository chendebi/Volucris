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
		window->Close();
	}

	void onGLFWWindowResized(GLFWwindow* handle, int w, int h)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		window->WindowSizeChanged(w, h);
	}

	void onGLFWFrameResized(GLFWwindow* handle, int w, int h)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		window->FrameSizeChanged(w, h);
	}

	void onGLFWMouseMove(GLFWwindow* handle, double w, double h)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		window->MouseMove(w, h);
	}

	Window::Window()
		: m_impl(new Impl)
		, m_size(800, 600)
		, m_frameSize(0,0)
		, m_title()
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
		m_size = { width, height };
		if (m_impl->handle)
		{
			glfwSetWindowSize(m_impl->handle, width, height);
		}
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

		m_impl->handle = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_impl->handle, this);
		glfwSetWindowCloseCallback(m_impl->handle, onGLFWWindowClose);
		glfwSetWindowSizeCallback(m_impl->handle, onGLFWWindowResized);
		glfwSetFramebufferSizeCallback(m_impl->handle, onGLFWFrameResized);
		glfwSetCursorPosCallback(m_impl->handle, onGLFWMouseMove);

		WindowSizeChanged.addLambda([this](int w, int h) {
			m_size = { w, h };
			});
		FrameSizeChanged.addLambda([this](int w, int h) {
			m_frameSize = { w, h };
			});
		glfwGetFramebufferSize(m_impl->handle, &m_frameSize.width, &m_frameSize.height);
		FrameSizeChanged(m_frameSize.width, m_frameSize.height);
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