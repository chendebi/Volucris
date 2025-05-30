#ifndef __volucris_level_h__
#define __volucris_level_h__

#include <memory>
#include <vector>
#include <Engine/Scene/scene.h>
#include <Engine/Core/rect.h>

namespace volucris
{
	class Scene;

	class Level : public Scene
	{
	public:
		Level();

		std::shared_ptr<ViewClient> addClient(const Rect& rect);

		std::shared_ptr<ViewClient> getClient(int index);

		void dispatchMousePressEvent(const MouseEvent& event);

		void dispatchMouseReleaseEvent(const MouseEvent& event);

		void dispatchMouseMoveEvent(int x, int y);

	private:
		std::vector<std::shared_ptr<ViewClient>> m_clients;
	};
}

#endif // !__volucris_level_h__
