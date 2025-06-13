#include "Core/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
#include <thread>

namespace volucris
{
	class LogMessageCallbackSink : public spdlog::sinks::base_sink<std::mutex> 
	{
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override 
		{
			spdlog::memory_buf_t formatted;
			formatter_->format(msg, formatted);
			// todo: 
		}

		void flush_() override {}
	};

	struct Logger::Impl
	{
		std::shared_ptr<spdlog::logger> logger = nullptr;
	};

	Logger::Logger(const char* name, Level level)
		: m_impl(new Impl)
	{
		std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;

#ifdef WITH_EDITOR
		auto sink = std::make_shared<LogMessageCallbackSink>();
		sinks.push_back(sink);
#endif // WITH_EDITOR

		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		//auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/mylog.txt");

		m_impl->logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
		m_impl->logger->set_level(spdlog::level::level_enum(level));
		spdlog::register_logger(m_impl->logger);
	}

	Logger::~Logger()
	{
		delete m_impl;
	}

	void Logger::log(Level level, const char* msg)
	{
		
	}
}
