#include "Application/application.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/window.h>
#include <crtdbg.h>
#include "Resource/resource_registry.h"

using namespace volucris;

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ResourceRegistry::Instance().loadManifiset();

	auto app = volucrisMain(argc, argv);
	checkq(Application::Instance(), Engine, "application not created");
	V_LOG_INFO(Engine, "launch application: {}", app->getAppConfig().appName);
	if (!app->initialize())
	{
		V_LOG_ERROR(Engine, "application initialize failed.");
		return -1;
	}

	app->exec();
	return 0;
}