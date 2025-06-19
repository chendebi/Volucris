#include "Profile/ScopedTimer.h"
#include <Profile/ProfileManager.h>

namespace volucris
{
	ScopedTimer::ScopedTimer()
		: m_scope()
		, m_name()
		, m_timer()
	{
	}

	ScopedTimer::ScopedTimer(std::string scope, std::string name)
		: m_scope(std::move(scope))
		, m_name(std::move(name))
		, m_timer()
	{
		
	}

	ScopedTimer::~ScopedTimer()
	{
		finish();
	}

	void ScopedTimer::registry()
	{
		if (!m_timer.isRunning())
		{
			ProfileManager::getInstance().push(this);
			m_timer.start();
		}
	}

	void ScopedTimer::finish()
	{
		if (m_timer.isRunning())
		{
			m_timer.stop();
			ProfileManager::getInstance().pop(m_timer);
		}
	}
}
