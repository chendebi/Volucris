#ifndef __volucris_scene_h__
#define __volucris_scene_h__

#include <memory>
#include <vector>
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

		~Scene();

		void addActor(const std::shared_ptr<Actor>& actor);

		void addViewport(const std::shared_ptr<Viewport>& viewport);

		void tick(const double& delta);

		void update();

		void attachToRenderer();

		void disattachFromRenderer();

		SceneProxy* getProxy() const { return m_proxy; }

		const std::vector<std::shared_ptr<Viewport>>& getViews() const
		{
			return m_views;
		}

	private:
		friend class Renderer;
		std::vector<std::shared_ptr<Actor>> m_actors;
		std::vector<std::shared_ptr<Viewport>> m_views;
		SceneProxy* m_proxy;
	};
}

#endif // !__volucris_scene_h__
