#include "Game/Region.h"

namespace volucris
{
	Region::Region()
		: GameObject()
		, m_world(nullptr)
		, m_entites()
	{

	}

	void Region::update()
	{
		for (auto& entity : m_entites)
		{
			//entity->
		}
	}
}
