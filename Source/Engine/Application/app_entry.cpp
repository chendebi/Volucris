#include "Application/application.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/window.h>

int main(int argc, char** argv)
{
	auto app = volucrisMain(argc, argv);
	checkq(volucris::Application::Instance(), Engine, "application not created");
	V_LOG_INFO(Engine, "launch application: {}", app->getAppConfig().appName);
	if (!app->initialize())
	{
		V_LOG_ERROR(Engine, "application initialize failed.");
		return -1;
	}

	app->exec();
	return 0;
}