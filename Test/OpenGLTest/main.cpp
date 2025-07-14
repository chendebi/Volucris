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

static uint32 pbo1, pbo2, vbo, ebo;

uint32 createBuffer(GLenum type, size_t size)
{
	uint32 buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(type, buffer);
	glBufferData(type, size, nullptr, GL_STATIC_DRAW);
	return buffer;
}

void debugBuffers()
{
	V_LOG_INFO(OpenGLTest, "----------------")
	V_LOG_INFO(OpenGLTest, "PBO 1: {}", pbo1)
	V_LOG_INFO(OpenGLTest, "PBO 1: {}", pbo2)
	V_LOG_INFO(OpenGLTest, "vbo: {}", vbo)
	V_LOG_INFO(OpenGLTest, "ebo: {}", ebo)
}

int main()
{
	auto window = init();

	pbo1 = createBuffer(GL_PIXEL_PACK_BUFFER, 8 * 8 * 3);
	pbo2 = createBuffer(GL_PIXEL_UNPACK_BUFFER, 8 * 8 * 3);
	vbo = createBuffer(GL_ARRAY_BUFFER, 128);
	ebo = createBuffer(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32));

	debugBuffers();

	glDeleteBuffers(1, &pbo1);
	glDeleteBuffers(1, &pbo2);

	pbo1 = createBuffer(GL_PIXEL_PACK_BUFFER, 128 * 8 * 3);
	pbo2 = createBuffer(GL_PIXEL_UNPACK_BUFFER, 128 * 8 * 3);

	debugBuffers();

	GLint id = 0;
	GLint vbo_id = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo_id);
	//v_check(vao_id == vao->getId());
	//v_check(vbo_id == vao->getId());
	v_check(id == ebo)

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}