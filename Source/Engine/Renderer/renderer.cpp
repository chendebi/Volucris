#include "Renderer/renderer.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Renderer/context.h"
#include "Renderer/viewport_proxy.h"
#include "Application/viewport.h"
#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Renderer/scene_proxy.h"
#include "Scene/scene.h"
#include <Core/assert.h>
#include "Core/vector_help.h"

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

		for (const auto& scene : m_scenes)
		{
			scene->update();
		}

		for (const auto& scene : m_scenes)
		{
			scene->render();
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

	void Renderer::addScene(const std::shared_ptr<SceneProxy>& scene)
	{
		pushCommand([this, scene]() {
			m_scenes.push_back(scene);
			});
	}

	void Renderer::removeScene(Scene* scene)
	{
		check(scene && scene->getSceneProxy() != nullptr);
		auto proxy = scene->getSceneProxy();
		pushCommand([this, proxy]() {
			VectorHelp::quickRemoveFirstIf<std::shared_ptr<SceneProxy>>(m_scenes, [proxy](const std::shared_ptr<SceneProxy>& scene)->bool {
				return scene.get() == proxy;
				});
			});
	}

	PrimitiveProxy* Renderer::createPrimitiveProxy(PrimitiveComponent* primitive)
	{
		auto proxy = std::make_shared<PrimitiveProxy>(primitive);
		auto scene = primitive->getScene()->getSceneProxy();
		pushCommand([scene, proxy]() {
			scene->addPrimitiveProxy(proxy);
			});
		return proxy.get();
	}

	void Renderer::pushCommand(RenderCommand command)
	{
		m_commands.push(std::move(command));
	}

	void Renderer::release()
	{
		m_scenes.clear();
	}

	void Renderer::clearCommands()
	{
		RenderCommand command;
		while (m_commands.pop(command))
		{
			
		}
	}

	void Renderer::renderui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
