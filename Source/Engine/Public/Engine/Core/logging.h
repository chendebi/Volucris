#ifndef __volucris_logging_h__
#define __volucris_logging_h__

#include "logger.h"
#include <fmt/format.h>

#define VOLUCRIS_LOGGER_NAME(name) __volucris_logger_##name
#define VOLUCRIS_DEFINE_LOG(name) extern volucris::Logger VOLUCRIS_LOGGER_NAME(name);
#define VOLUCRIS_DECLARE_LOG(name, level) volucris::Logger VOLUCRIS_LOGGER_NAME(name) = volucris::Logger(#name, volucris::Logger::Level::level);

#define V_LOG(name, level, format_s, ...) VOLUCRIS_LOGGER_NAME(name).log(fmt::format(format_s, ##__VA_ARGS__), volucris::Logger::Level::level);
#define V_LOG_TRACE(name, format_s, ...) V_LOG(name, Trace, format_s, ##__VA_ARGS__)
#define V_LOG_DEBUG(name, format_s, ...) V_LOG(name, Debug, format_s, ##__VA_ARGS__)
#define V_LOG_INFO(name, format_s, ...) V_LOG(name, Info, format_s, ##__VA_ARGS__)
#define V_LOG_WARN(name, format_s, ...) V_LOG(name, Warn, format_s, ##__VA_ARGS__)
#define V_LOG_ERROR(name, format_s, ...) V_LOG(name, Error, format_s, ##__VA_ARGS__)
#define V_LOG_CRITICAL(name, format_s, ...) V_LOG(name, Critical, format_s, ##__VA_ARGS__)

#endif // __volucris_logging_h__