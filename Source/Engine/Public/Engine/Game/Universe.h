#ifndef __volucris_universe_h__
#define __volucris_universe_h__

#include <Engine/Core/Object.h>

namespace volucris
{
	class Scene;

	class Universe : public Object
	{
	public:
		Universe();

		virtual ~Universe();

		void update();
		
	protected:
		
	private:
		std::weak_ptr<Scene> m_scene;
	};
}

#endif // !__volucris_universe_h__
