#ifndef __volucris_scoped_timer_h__
#define __volucris_scoped_timer_h__

#include <Engine/Core/Timer.h>
#include <string>

namespace volucris
{
	class ScopedTimer
	{
	public:
		ScopedTimer();

		ScopedTimer(std::string scope, std::string name);
		
		~ScopedTimer();

		void setName(const std::string& scope, const std::string& name)
		{
			m_scope = scope;
			m_name = name;
		}

		void registry();

		void finish();

		const std::string& getScopeName() const { return m_scope; }

		const std::string& getName() const { return m_name; }

		const Timer& getTimer() const { return m_timer; }

	private:
		std::string m_scope;
		std::string m_name;
		Timer m_timer;
	};
}

#endif // !__volucris_scoped_timer_h__
