#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Engine/Core/Logging.h>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Core/Assert.h>

VOLUCRIS_STATIC_LOG(OpenGLTest, Trace)


#define GL_CHECK() \
	{\
		auto err = glGetError(); \
		while (err != GL_NO_ERROR) \
		{ \
			v_checkf(false, OpenGLTest, "gl error at {} : {}, code: {:X}", __FILE__, __LINE__, err); \
			err = glGetError(); \
		} \
	}

using namespace volucris;

GLFWwindow* init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	auto window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
	if (!window)
	{
		V_LOG_ERROR(OpenGLTest, "Failed to create GLFW window");
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	return window;
}

static uint32 texID = 0;

int main()
{
	auto window = init();

	glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
		V_LOG_INFO(OpenGLTest, "Window resized to {}x{}", width, height);
		if (texID > 0)
		{
			glDeleteTextures(1, &texID);
			texID = 0;
		}
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
		GL_CHECK();
		});

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}