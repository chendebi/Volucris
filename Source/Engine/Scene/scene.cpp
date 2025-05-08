#include "Scene/scene.h"
#include "Scene/actor.h"
#include "Core/volucris.h"
#include "Application/viewport.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Renderer/scene_proxy.h"
#include "Renderer/viewport_proxy.h"

namespace volucris
{
	Scene::Scene()
		: m_actors()
		, m_proxy(nullptr)
	{
	}

	Scene::~Scene()
	{
		RenderStateChanged.clear();
		for (const auto& view : m_views)
		{
			view->disattach();
		}

		for (const auto& actor : m_actors)
		{
			actor->disattach();
		}
	}

	void Scene::addActor(const std::shared_ptr<Actor>& actor)
	{
		if (actor->isAttached())
		{
			V_LOG_WARN(Engine, "actor has been attached");
			return;
		}
		m_actors.push_back(actor);
		actor->attach(this);
	}

	void Scene::addViewport(const std::shared_ptr<Viewport>& viewport)
	{
		if (viewport->isAttached())
		{
			V_LOG_WARN(Engine, "actor has been attached");
			return;
		}
		m_views.push_back(viewport);
		viewport->attach(this);
	}

	void Scene::tick(const double& delta)
	{
		for (const auto& actor : m_actors)
		{
			actor->tick(delta);
		}
	}

	void Scene::update()
	{
		for (const auto& actor : m_actors)
		{
			actor->update();
		}
	}

	void Scene::attachToRenderer()
	{
		auto renderer = gApp->getRenderer();
		auto sceneProxy = std::make_shared<SceneProxy>(this);

		for (const auto& view : m_views)
		{
			auto proxy = std::make_shared<ViewportProxy>(view.get());
			view->m_proxy = proxy.get();
			sceneProxy->addViewportProxy(proxy);
		}
		m_proxy = renderer->addScene(this);
		V_LOG_DEBUG(Engine, "scene attach to renderer");
		RenderStateChanged.broadcast();
	}

	void Scene::disattachFromRenderer()
	{
		auto renderer = gApp->getRenderer();
		m_proxy = nullptr;
		RenderStateChanged.broadcast();
		for (const auto& view : m_views)
		{
			view->m_proxy = nullptr;
		}
		renderer->removeScene(this);
		V_LOG_DEBUG(Engine, "scene disattach from renderer");
	}
}
