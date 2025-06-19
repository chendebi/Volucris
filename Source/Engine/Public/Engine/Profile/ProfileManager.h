#ifndef __volucris_profile_manager_h__
#define __volucris_profile_manager_h__

#include <string>
#include <vector>
#include <memory>
#include "Timer.h"

namespace volucris
{
	struct ProfileScope
	{
		std::string scopeName;
		std::string name;
		Timer timer;
		ProfileScope* parent = nullptr;
		std::vector<std::unique_ptr<ProfileScope>> scopes;
	};

	class ScopedTimer;

	class ProfileManager
	{
	public:
		~ProfileManager();

		static ProfileManager& getInstance()
		{
			static ProfileManager inst;
			return inst;
		}

		void setProfileEnabled(bool enabled)
		{
			m_profileEnabled = enabled;
		}

		void push(ScopedTimer* timer);

		void pop(const Timer& timer);

		bool isProfiling() const { return m_profiling; }

		void print();

	private:
		friend class Application;
		void initialize()
		{
			m_profiling = m_profileEnabled;
		}

	protected:
		ProfileManager();
		ProfileManager(const ProfileManager&) = delete;
		ProfileManager(ProfileManager&&) = delete;
		ProfileManager& operator=(const ProfileManager&) = delete;
		ProfileManager& operator=(ProfileManager&&) = delete;

	private:
		std::vector<std::unique_ptr<ProfileScope>> m_scopes;
		std::vector<ProfileScope*> m_stack;
		bool m_profiling;
		bool m_profileEnabled;
	};
}

#endif // !__volucris_profile_manager_h__
