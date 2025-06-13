#ifndef __volucris_logger_h__
#define __volucris_logger_h__

namespace volucris
{
	class Logger
	{
	public:
		enum class Level
		{
			Trace,
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};
		
	public:
		Logger(const char* name, Level level = Level::Info);

		~Logger();

		void log(Level level, const char* msg);

		void trace(const char* msg) { log(Level::Trace, msg); }
		void debug(const char* msg) { log(Level::Debug, msg); }
		void info(const char* msg) { log(Level::Info, msg); }
		void warn(const char* msg) { log(Level::Warning, msg); }
		void error(const char* msg) { log(Level::Error, msg); }
		void critical(const char* msg) { log(Level::Critical, msg); }

	private:
		struct Impl;
		Impl* m_impl;
	};
}

#endif // !__volucris_logger_h__
