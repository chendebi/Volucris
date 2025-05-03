#include "Application/window.h"
#include <Core/types_help.h>
#include <GLFW/glfw3.h>
#include "Application/application.h"

namespace volucris
{
	struct Window::Impl
	{
		uint8 inited = false;
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
		if (m_impl->inited)
		{
			destroy();
		}
		delete m_impl;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
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
			glfwDestroyWindow(m_impl->handle);
			m_impl->inited = false;
		}
	}

	void* Window::getHandle() const
	{
		return m_impl->handle;
	}
}