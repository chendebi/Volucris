#ifndef __volucris_game_object_h__
#define __volucris_game_object_h__

#include <Engine/Core/Object.h>
#include <vector>

namespace volucris
{
	class GameObject : public Object
	{
	public:
		GameObject();

		~GameObject() override;

		void setOuter(GameObject* outer);

	private:
		GameObject* m_outer;
		std::vector< std::shared_ptr<GameObject>> m_objects;
	};
}

#endif // !__volucris_game_object_h__
