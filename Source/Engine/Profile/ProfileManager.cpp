#include <Profile/ProfileManager.h>
#include <Profile/ScopedTimer.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fmt/format.h>
#include <Core/Volucris.h>
#include <iostream>

namespace volucris
{
	ProfileManager::ProfileManager()
		: m_scopes()
		, m_stack()
		, m_profiling(false)
		, m_profileEnabled(false)
	{

	}

	ProfileManager::~ProfileManager()
	{

	}

	void volucris::ProfileManager::push(ScopedTimer* timer)
	{
		if (!m_profiling)
		{
			return;
		}

		if (m_stack.empty())
		{
			m_scopes.push_back(std::make_unique<ProfileScope>());
			auto current = m_scopes.rbegin()->get();

			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);
			std::tm tm = *std::localtime(&time);

			std::ostringstream oss;
			oss << std::put_time(&tm, "%H-%M-%S");

			current->name = fmt::format("Frame_{}_{}", m_scopes.size(), oss.str());
			m_stack.push_back(current);
		}

		auto current = *m_stack.rbegin();
		if (current->scopeName == timer->getScopeName())
		{
			auto parent = current->parent;
			parent->scopes.push_back(std::make_unique<ProfileScope>());
			current = parent->scopes.rbegin()->get();
			current->scopeName = timer->getScopeName();
			current->name = timer->getName();
			current->parent = parent;
			m_stack.push_back(current);
		}
		else
		{
			auto parent = current;
			current->scopes.push_back(std::make_unique<ProfileScope>());
			current = current->scopes.rbegin()->get();
			current->name = timer->getName();
			current->scopeName = timer->getScopeName();
			current->parent = parent;
			m_stack.push_back(current);
		}
	}

	void ProfileManager::pop(const Timer& timer)
	{
		if (m_stack.empty() || !m_profiling)
		{
			return;
		}
		auto current = *m_stack.rbegin();
		current->timer = timer;
		m_stack.pop_back();
		if (m_stack.size() == 1)
		{
			m_stack.pop_back();
		}
	}

	void logScope(ProfileScope* scope, int space)
	{
		for (int i = 0; i < space; ++i)
		{
			std::cout << " ";
		}
		std::cout << scope->scopeName << "[" << scope->name << "] : " << scope->timer.getDuration() << "\n";
		for (auto& child : scope->scopes)
		{
			logScope(child.get(), space + 4);
		}
	}

	void ProfileManager::print()
	{
		for (auto& scope : m_scopes)
		{
			//V_LOG_DEBUG(Engine, "{}", scope.name);
			auto space = 0;
			logScope(scope.get(), 0);
		}
	}
}