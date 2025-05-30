#include "Scene/scene.h"
#include "Scene/actor.h"
#include "Core/volucris.h"
#include "Scene/view_client.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Renderer/scene_proxy.h"
#include "Renderer/viewport_proxy.h"
#include <Scene/camera_component.h>

namespace volucris
{
	Scene::Scene()
		: m_dirty(true)
		, m_actors()
		, m_views()
		, m_ids()
		, m_recycledIds()
		, m_proxy()
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

	void Scene::addViewClient(const std::shared_ptr<ViewClient>& client)
	{
		if (client->isAttached())
		{
			V_LOG_WARN(Engine, "client has been attached");
			return;
		}
		m_views.push_back(client);
		client->attach(this);
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
		if (m_dirty)
		{
			std::vector<std::shared_ptr<ViewProxy>> updateViews;
			for (const auto& view : m_views)
			{
				if (!view->isRenderProxyCreated())
				{
					updateViews.push_back(view->getProxy());
				}
			}

			if (!updateViews.empty())
			{
				auto proxy = getSceneProxy();
				gApp->getRenderer()->pushCommand([proxy, updateViews]() {
					proxy->addViews(updateViews);
					});
			}
		}

		for (const auto& view : m_views)
		{
			view->update();
		}

		for (const auto& actor : m_actors)
		{
			actor->update();
		}
	}

	void Scene::attachToRenderer()
	{
		if (!m_proxy.expired())
		{
			V_LOG_ERROR(Engine, "scene has been attached");
			return;
		}

		auto renderer = gApp->getRenderer();
		auto proxy = getSceneProxy();
		std::vector<std::shared_ptr<ViewProxy>> updateViews;
		for (const auto& view : m_views)
		{
			updateViews.push_back(view->getProxy());
		}
		proxy->addViews(updateViews);
		renderer->addScene(proxy);
		V_LOG_DEBUG(Engine, "scene attach to renderer");
		RenderStateChanged.broadcast();
	}

	void Scene::deattachFromRenderer()
	{
		auto renderer = gApp->getRenderer();
		renderer->removeScene(this);
		RenderStateChanged.broadcast();
		update();
		V_LOG_DEBUG(Engine, "scene disattach from renderer");
	}

	std::shared_ptr<SceneProxy> Scene::getSceneProxy()
	{
		std::shared_ptr<SceneProxy> proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<SceneProxy>();
			proxy->initialize(this);
			m_proxy = proxy;
		}
		return proxy;
	}

	std::string Scene::getDefaultDisplayName(const std::string& key)
	{
		std::string name;
		{
			auto it = m_recycledIds.find(key);
			if (it != m_recycledIds.end() && !it->second.empty())
			{
				name = fmt::format("{}_{}", key, *it->second.rbegin());
				it->second.pop_back();
				return name;
			}
		}

		auto it = m_ids.find(key);
		size_t id = 0;
		if (it != m_ids.end())
		{
			id = it->second;
		}

		name = fmt::format("{}_{}", key, id);
		m_ids[key] = ++id;
		return name;
	}
}
