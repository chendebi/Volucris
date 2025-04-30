#include "Core/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace volucris
{
	Logger::Logger(const std::string& name, Level level)
		: m_logger(nullptr)
	{
		m_logger = spdlog::stdout_color_mt(name);
		m_logger->set_level(spdlog::level::level_enum(level));
	}

	void Logger::log(const std::string& msg, Level level)
	{
		m_logger->log(spdlog::level::level_enum(level), msg);
	}
}