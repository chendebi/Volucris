#include "Renderer/renderer.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Renderer/context.h"

namespace volucris
{
	void Renderer::render()
	{

		renderui();
		m_context->swapBuffers();
	}

	void Renderer::setContext(const std::shared_ptr<Context>& context)
	{
		m_context = context;
	}

	void Renderer::renderui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
