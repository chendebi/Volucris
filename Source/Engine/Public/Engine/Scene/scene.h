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
	class Viewport;

	class Scene
	{
	public:
		OnRenderStateChanged RenderStateChanged;

	public:
		Scene();

		virtual ~Scene();

		void addActor(const std::shared_ptr<Actor>& actor);

		void addViewport(const std::shared_ptr<Viewport>& viewport);

		void tick(const double& delta);

		void update();

		void attachToRenderer();

		void disattachFromRenderer();

		SceneProxy* getSceneProxy() const { return m_proxy; }

		const std::vector<std::shared_ptr<Viewport>>& getViews() const
		{
			return m_views;
		}

		std::string getDefaultDisplayName(const std::string& key);

	private:
		friend class Renderer;
		std::vector<std::shared_ptr<Actor>> m_actors;
		std::vector<std::shared_ptr<Viewport>> m_views;
		
		std::unordered_map<std::string, int> m_ids;
		std::unordered_map<std::string, std::vector<int>> m_recycledIds;
		SceneProxy* m_proxy;
	};
}

#endif // !__volucris_scene_h__
