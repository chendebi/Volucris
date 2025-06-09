#include "Scene/level.h"
#include <Resource/render_target.h>
#include <Scene/view_client.h>

namespace volucris
{
	Level::Level()
		: Scene()
	{
		addClient({ 0,0,0,0 });
	}

	std::shared_ptr<ViewClient> Level::addClient(const Rect& rect)
	{
		auto target = std::make_shared<RenderTarget>(true);
		auto client = std::make_shared<ViewClient>(target);
		m_clients.push_back(client);
		addViewClient(client);
		return client;
	}

	std::shared_ptr<ViewClient> Level::getClient(int index)
	{
		if (0 < index && index < m_clients.size())
		{
			return m_clients[index];
		}
		return nullptr;
	}


	void Level::dispatchEvent(ClientEvent* e)
	{
		for (const auto& client : m_clients)
		{
			if (client->isHardwareClient())
			{
				client->dispatchEvent(e);
			}
		}
	}

}
