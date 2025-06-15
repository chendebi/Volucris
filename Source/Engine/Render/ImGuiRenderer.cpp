#include "Render/ImGuiRenderer.h"
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <Core/Volucris.h>

namespace volucris
{
	bool s_gladInitialized = false;

	ImGuiRenderer::ImGuiRenderer(GLFWwindow* handle)
		: Object(), m_windowHandle(handle), m_imguiContext(nullptr)
	{
		glfwMakeContextCurrent(handle);

		if (!s_gladInitialized)
		{
			gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

			{
				const auto vender = glGetString(GL_VENDOR);
				const auto renderer = glGetString(GL_RENDERER);
				const auto language = glGetString(GL_SHADING_LANGUAGE_VERSION);
				const auto version = glGetString(GL_VERSION);

				V_LOG_INFO(Engine, "context initialized");
				V_LOG_INFO(Engine, "	vender: {}", (char*)vender);
				V_LOG_INFO(Engine, "	renderer: {}", (char*)renderer);
				V_LOG_INFO(Engine, "	version: {}", (char*)version);
				V_LOG_INFO(Engine, "	language version: {}", (char*)language);
			}

			s_gladInitialized = true;
		}

		IMGUI_CHECKVERSION();
		m_imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_imguiContext);
		ImGui_ImplGlfw_InitForOpenGL(handle, true);
		if (!ImGui_ImplOpenGL3_Init("#version 330"))
		{
			V_LOG_CRITICAL(Engine, "imgui initialize failed");
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui::SetCurrentContext(m_imguiContext);
		ImGui_ImplGlfw_Shutdown();

		// 2. 再清理 ImGui 的其他后端（如 OpenGL/Vulkan）
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext(m_imguiContext);
	}

	void ImGuiRenderer::render()
	{
		ImGui::SetCurrentContext(m_imguiContext);
		glfwMakeContextCurrent(m_windowHandle);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(m_windowHandle, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_windowHandle);
	}

	void ImGuiRenderer::makeCurrent()
	{
		ImGui::SetCurrentContext(m_imguiContext);
	}
}
