#ifndef __volucris_logger_h__
#define __volucris_logger_h__

#include <string>
#include <memory>

namespace spdlog
{
	class logger;
}

namespace volucris
{
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
		Logger(const std::string& name, Level level = Trace);

		static Logger& Instance()
		{
			static Logger inst("volucris");
			return inst;
		}

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
