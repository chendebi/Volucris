#include "Core/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

namespace volucris
{
	CircleQueue<std::string> gLogBuffer(1024);

	class CallbackLogSink : public spdlog::sinks::base_sink<std::mutex> {
	public:

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override {
			spdlog::memory_buf_t formatted;
			formatter_->format(msg, formatted);
			if (gLogBuffer.count() == gLogBuffer.capacity())
			{
				gLogBuffer.removeFirst();
			}
			gLogBuffer.push(fmt::to_string(formatted));
		}

		void flush_() override {}
	};

	Logger::Logger()
		: m_logger(nullptr)
	{
	}

	Logger::Logger(const std::string& name, Level level)
	{
		initialize(name, level);
	}

	void Logger::initialize(const std::string& name, Level level)
	{
		if (m_logger)
		{
			return;
		}

		std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
		auto sink = std::make_shared<CallbackLogSink>();
		sinks.push_back(sink);

		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		//auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/mylog.txt");

		m_logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
		m_logger->set_level(spdlog::level::level_enum(level));
		spdlog::register_logger(m_logger);
	}

	void Logger::log(const std::string& msg, Level level)
	{
		m_logger->log(spdlog::level::level_enum(level), msg);
	}
}