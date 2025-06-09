#include "Application/window.h"
#include <Core/types_help.h>
#include <GLFW/glfw3.h>
#include "Application/application.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <Core/volucris.h>

#include <Windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Resource/resource_registry.h>

namespace volucris
{
	struct Window::Impl
	{
		uint8 imguiInited = false;
		GLFWwindow* handle = nullptr;
	};

	inline MouseEvent::MouseButton glfwButtonToMouseButton(int button)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			return MouseEvent::MouseButton::LEFT;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			return MouseEvent::MouseButton::MIDDLE;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			return MouseEvent::MouseButton::RIGHT;
		}
		else
		{
			V_LOG_ERROR(Engine, "recieved a unsupported glfw button: {}", button);
		}
		return MouseEvent::MouseButton::UNKNOWN;
	}

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
		MouseMoveEvent e = MouseMoveEvent(w, h);
		window->WindowEvent(&e);
	}

	void onGLFWMouseButton(GLFWwindow* handle, int button, int action, int mods)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		if (action == GLFW_PRESS)
		{
			auto e = MousePressEvent(glfwButtonToMouseButton(button));
			e.modifiers = mods;
			window->WindowEvent(&e);
		}
		else
		{
			auto e = MouseReleaseEvent(glfwButtonToMouseButton(button));
			e.modifiers = mods;
			window->WindowEvent(&e);
		}
	}

	void onGLFWKeyEvent(GLFWwindow* handle, int key, int scancode, int action, int mods)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			auto e = KeyPressEvent((Key)key, mods);
			window->WindowEvent(&e);
		}
		else
		{
			auto e = KeyReleaseEvent((Key)key, mods);
			window->WindowEvent(&e);
		}

	}

	Window::Window()
		: m_impl(new Impl)
		, m_rect(100,100, 800, 600)
		, m_normalRect(100,100,800, 600)
		, m_frameSize(0,0)
		, m_title()
		, m_fullScreen(0)
		, m_frameless(0)
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

	void Window::setPosition(int x, int y)
	{
		m_rect.x = x;
		m_rect.y = y;
		if (m_impl->handle)
		{
			glfwSetWindowPos(m_impl->handle, x, y);
		}
	}

	void Window::setSize(int width, int height)
	{
		m_rect.w = width;
		m_rect.h = height;
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

		m_impl->handle = glfwCreateWindow(m_rect.w, m_rect.h, m_title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_impl->handle, this);
		glfwSetWindowCloseCallback(m_impl->handle, onGLFWWindowClose);
		glfwSetWindowSizeCallback(m_impl->handle, onGLFWWindowResized);
		glfwSetFramebufferSizeCallback(m_impl->handle, onGLFWFrameResized);
		glfwSetCursorPosCallback(m_impl->handle, onGLFWMouseMove);
		glfwSetMouseButtonCallback(m_impl->handle, onGLFWMouseButton);
		glfwSetKeyCallback(m_impl->handle, onGLFWKeyEvent);

		if (m_frameless)
		{
			glfwSetWindowAttrib(m_impl->handle, GLFW_DECORATED, false);
		}

		if (m_fullScreen)
		{
			setFullScreen(true);
		}

		WindowSizeChanged.addLambda([this](int w, int h) {
			m_rect.w = w;
			m_rect.h = h;
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
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
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

		ImFontConfig config;
		config.OversampleH = 2; // 提高水平采样质量
		config.OversampleV = 1; // 垂直采样

		std::string fontPath;
		ResourceRegistry::Instance().getSystemPathByResourcePath("/Engine/Font/wenquanyi.ttf", fontPath);
		// 加载英文字体（主字体）
		ImFont* font_en = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f, &config);

		// 加载中文字体（合并到同一字体集）
		//static const ImWchar ranges[] = { 0x4E00, 0x9FFF, 0 }; // 基本汉字范围
		//ImFont* font_cn = io.Fonts->AddFontFromFileTTF(fontPath, 18.0f, &config, ranges);

		// 必须重建字体纹理
		//ImGui_ImplOpenGL3_CreateFontsTexture();

		// Setup Platform/Renderer backends

		float dpi_scale = 1.0f;
#if defined(_WIN32)
		// Windows获取DPI缩放
		dpi_scale = (float)GetDpiForSystem() / 96.0f;
#elif defined(__APPLE__)
		// macOS获取Retina缩放因子
#endif

		ImGui::GetIO().FontGlobalScale = dpi_scale;
		ImGui::GetStyle().ScaleAllSizes(dpi_scale);

		ImGui_ImplGlfw_InitForOpenGL(m_impl->handle, true);
		ImGui_ImplOpenGL3_Init(glslversion.c_str());
		m_impl->imguiInited = true;
		return true;
	}

	void Window::setFullScreen(bool enabled)
	{
		m_fullScreen = enabled;
		if (!m_impl->handle)
		{
			return;
		}

		glfwMaximizeWindow(m_impl->handle);
	}

	void Window::setFrameless(bool enabled)
	{
		if (enabled)
		{
			glfwSetWindowAttrib(m_impl->handle, GLFW_DECORATED, false);
		}
		else
		{
			glfwSetWindowAttrib(m_impl->handle, GLFW_DECORATED, true);
		}
	}

	void Window::setCursorEnabled(bool enabled)
	{
		if (m_impl->handle)
		{
			glfwSetInputMode(m_impl->handle, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		}
	}

	Point2D Window::getMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(m_impl->handle, &x, &y);
		return { (int)x, (int)y };
	}
}