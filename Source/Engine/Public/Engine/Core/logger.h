#ifndef __volucris_logger_h__
#define __volucris_logger_h__

#include <string>
#include <memory>
#include <functional>
#include <Engine/Core/circle_queue.h>

namespace spdlog
{
	class logger;
}

namespace volucris
{
	extern CircleQueue<std::string> gLogBuffer;

	class LogBuffer;
	class Logger
	{
	public:
		enum Level
		{
			Trace,
			Debug,
			Info,
			Warn,
			Error,
			Critical
		};

	public:
		Logger();

		Logger(const std::string& name, Level level = Trace);

		void initialize(const std::string& name, Level level = Trace);

		void log(const std::string& msg, Level level);

		void trace(const std::string& msg) { log(msg, Trace); }
		
		void debug(const std::string& msg) { log(msg, Debug); }
		
		void info(const std::string& msg) { log(msg, Info); }
		
		void warn(const std::string& msg) { log(msg, Warn); }
		
		void error(const std::string& msg) { log(msg, Error); }
		
		void critical(const std::string& msg) { log(msg, Critical); }

	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};
}

#endif // !__be_logger_h__
