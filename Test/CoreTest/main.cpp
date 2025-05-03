#include <Engine/Core/logging.h>
#include <Engine/Core/assert.h>
#include <Engine/Application/application.h>

VOLUCRIS_DECLARE_LOG(CoreTest, Trace)

VOLUCRIS_DEFINE_LOG(CoreTest)

using namespace volucris;

//int main(int argc, char** argv)
//{
//	V_LOG_TRACE(CoreTest, "This is a trace message");
//	V_LOG_DEBUG(CoreTest, "This is a debug message");
//	V_LOG_INFO(CoreTest, "This is an info message");
//	V_LOG_WARN(CoreTest, "This is a warning message");
//	V_LOG_ERROR(CoreTest, "This is an error message");
//	V_LOG_CRITICAL(CoreTest, "This is a critical message");
//	//checkf(false, CoreTest, "this is fail")
//}

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	auto config = Application::Config(argc, argv);
	auto app = std::make_shared<Application>(config);
	return app;
}