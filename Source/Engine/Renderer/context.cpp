#include "Renderer/context.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Application/application.h"
#include "Application/window.h"

namespace volucris
{
	struct Context::Impl
	{
		GLFWwindow* window;
	};

	Context::Context()
		: m_impl(new Impl)
	{
		auto window = Application::Instance()->getWindow();
		m_impl->window = static_cast<GLFWwindow*>(window->getHandle());
		makeCurrent();
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Context::~Context()
	{
		delete m_impl;
	}

	void Context::makeCurrent()
	{
		glfwMakeContextCurrent(m_impl->window);
	}

	void Context::swapBuffers()
	{
		glfwSwapBuffers(m_impl->window);
	}
}
