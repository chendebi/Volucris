#include <Engine/Core/logging.h>
#include <Engine/Core/assert.h>

VOLUCRIS_DECLARE_LOG(CoreTest, Trace)

VOLUCRIS_DEFINE_LOG(CoreTest)

int main()
{
	V_LOG_TRACE(CoreTest, "This is a trace message");
	V_LOG_DEBUG(CoreTest, "This is a debug message");
	V_LOG_INFO(CoreTest, "This is an info message");
	V_LOG_WARN(CoreTest, "This is a warning message");
	V_LOG_ERROR(CoreTest, "This is an error message");
	V_LOG_CRITICAL(CoreTest, "This is a critical message");
	//checkf(false, CoreTest, "this is fail")
}