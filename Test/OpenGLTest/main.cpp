#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Engine/Core/Logging.h>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Core/Assert.h>
#include <vector>

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

int main()
{
	auto window = init();

	std::vector<uint8> data;
	data.resize(128 * 128 * 3);

	uint32 id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	GL_CHECK();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}