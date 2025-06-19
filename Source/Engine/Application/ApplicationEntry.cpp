#include <Core/Volucris.h>
#include <Application/Application.h>
#include <GLFW/glfw3.h>
#include <Application/Window.h>
#include <FileSystem/FileSystem.h>
#include <tracy/Tracy.hpp>

using namespace volucris;

bool initialize()
{
	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return true;
}

int main(int argc, char* argv[])
{
#ifdef TRACY_ENABLE
	//TracyGpuContext; // GPU上下文初始化
#endif

	gFileSystem.mount("/Engine/Content", fmt::format("{}/Content", VOLUCRIS_ENGINE_ROOT));
	gFileSystem.mount("/Engine/Config", fmt::format("{}/Binaries/Config", VOLUCRIS_ENGINE_ROOT));

	if (!initialize())
	{
		V_LOG_CRITICAL(Engine, "Failed to initialize GLFW");
		return -1;
	}

	auto app = volucrisEntry(argc, argv);

	return app->exec();
}