#include "Renderer/renderer.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Renderer/context.h"
#include "Renderer/viewport_proxy.h"
#include "Application/viewport.h"

namespace volucris
{
	Renderer::Renderer()
		: m_context(nullptr)
		, m_commands(1024)
	{ }

	Renderer::~Renderer()
	{
	}

	void Renderer::render()
	{
		RenderCommand command;
		while (m_commands.pop(command))
		{
			command();
		}

		for (const auto& viewport : m_viewports)
		{
			viewport->render();
		}

		renderui();
		m_context->swapBuffers();
	}

	void Renderer::setContext(const std::shared_ptr<Context>& context)
	{
		m_context = context;
	}

	void Renderer::setWindowFrameSize(int width, int height)
	{
		pushCommand([this, width, height]() {
			m_windowFrameSize = { width, height };
			});
	}

	void Renderer::setViewports(const std::vector<std::shared_ptr<Viewport>>& viewports)
	{
		std::vector<std::shared_ptr<ViewportProxy>> viewProxies;
		for (const auto& viewport : viewports)
		{
			viewport->createProxy();
			auto proxy = viewport->getProxy();
			viewProxies.push_back(proxy);
		}

		pushCommand([this, viewProxies]()
			{
				m_viewports = viewProxies;
			});
	}

	void Renderer::pushCommand(RenderCommand command)
	{
		m_commands.push(std::move(command));
	}

	void Renderer::renderui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
