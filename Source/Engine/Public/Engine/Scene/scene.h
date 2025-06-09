#ifndef __volucris_scene_h__
#define __volucris_scene_h__

#include <memory>
#include <vector>
#include <string>
#include "Engine/Core/event.h"

DECLARE_EVENT(OnRenderStateChanged)

namespace volucris
{
	class Actor;
	class SceneProxy;
	class ViewClient;

	class Scene
	{
	public:
		OnRenderStateChanged RenderStateChanged;

	public:
		Scene();

		virtual ~Scene();

		void addActor(const std::shared_ptr<Actor>& actor);

		template<typename T, typename ...Args>
		std::shared_ptr<T> addActor(Args&& ...args)
		{
			auto comp = std::make_shared<T>(std::move(std::forward<Args>(args)...));
			auto actor = std::make_shared<Actor>();
			actor->setRootComponent(comp);
			addActor(actor);
			return comp;
		}

		void addViewClient(const std::shared_ptr<ViewClient>& client);

		void tick(const double& delta);

		void update();

		void attachToRenderer();

		void deattachFromRenderer();

		std::shared_ptr<SceneProxy> getSceneProxy();

		const std::vector<std::shared_ptr<ViewClient>>& getViews() const
		{
			return m_views;
		}

		std::string getDefaultDisplayName(const std::string& key);

	private:
		friend class Renderer;
		bool m_dirty;
		std::vector<std::shared_ptr<Actor>> m_actors;
		std::vector<std::shared_ptr<ViewClient>> m_views;
		std::unordered_map<std::string, int> m_ids;
		std::unordered_map<std::string, std::vector<int>> m_recycledIds;
		std::weak_ptr<SceneProxy> m_proxy;
	};
}

#endif // !__volucris_scene_h__
