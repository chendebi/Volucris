#include "Render/ImGuiRenderer.h"
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <Core/Volucris.h>
#include <FileSystem/FileSystem.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Profile/Profile.h>
#include <tracy/Tracy.hpp>
#include <Application/Application.h>

#include <Engine/RHI/RHICommandList.h>
#include <Engine/Application/Window.h>
#include <backends/imgui_impl_glfw.cpp>

namespace volucris
{
	bool s_gladInitialized = false;

	ImGuiRenderer::ImGuiRenderer(Window* window)
		: m_imguiContext(nullptr)
		, m_cmdList(std::make_unique<RHICommandList>())
		, m_clear()
		, m_window(window)
	{
		m_cmdList->initialize(window, true);

		IMGUI_CHECKVERSION();
		m_imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_imguiContext);
		ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), false);
		if (!ImGui_ImplOpenGL3_Init("#version 430"))
		{
			V_LOG_CRITICAL(Engine, "imgui initialize failed");
		}

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		auto config = Application::config();
		std::string font = config.getValue("font/family", "/Engine/Content/Font/wenquanyi.ttf");
		float fontSize = config.getValue("font/size", 16.0f);
		io.Fonts->AddFontFromFileTTF(
			gFileSystem.virtualToPhysical(font).c_str(),
			fontSize,                         
			nullptr,
			io.Fonts->GetGlyphRangesChineseFull()
		);

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

		float dpi_scale = 1.0f;
#if defined(_WIN32)
		// Windows获取DPI缩放
		dpi_scale = (float)GetDpiForSystem() / 96.0f;
#elif defined(__APPLE__)
		// macOS获取Retina缩放因子
#endif

		io.FontGlobalScale = dpi_scale;
		ImGui::GetStyle().ScaleAllSizes(dpi_scale);
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		m_cmdList = nullptr;

		
		ImGui_ImplGlfw_Shutdown();

		// 2. 再清理 ImGui 的其他后端（如 OpenGL/Vulkan）
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext(m_imguiContext);
	}

	void ImGuiRenderer::render()
	{
		V_SCOPED_PROFILE;
		
		m_cmdList->setViewport(0, 0, 800, 600);
		m_cmdList->clear(m_clear);

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		m_cmdList->swapBuffers();
	}

	void ImGuiRenderer::makeCurrent()
	{
		V_LOG_DEBUG(Engine, "make context current for window: {}", m_window->getTitle());
		ImGui::SetCurrentContext(m_imguiContext);
		m_cmdList->makeCurrent();
	}

	void ImGuiRenderer::installedCallbacks(bool install)
	{
		ImGui_ImplGlfw_Data* bd = (ImGui_ImplGlfw_Data*)m_imguiContext->IO.BackendPlatformUserData;
		if (install && !bd->InstalledCallbacks)
		{
			ImGui_ImplGlfw_InstallCallbacks(m_window->getHandle());
		}
		else if (bd->InstalledCallbacks)
		{
			ImGui_ImplGlfw_RestoreCallbacks(m_window->getHandle());
		}
	}

	RHICommandList* ImGuiRenderer::getCommandList() const
	{
		return m_cmdList.get();
	}
}
