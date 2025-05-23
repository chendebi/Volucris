#include "Application/application.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/window.h>
#include <Core/directory.h>
#include <crtdbg.h>
#include "Resource/resource_registry.h"

using namespace volucris;

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ResourceRegistry::Instance().addResourceSearchPath((Directory::EngineDirectory() / "Content").toString(), "/Engine/");
	ResourceRegistry::Instance().addResourceSearchPath((Directory::EngineDirectory() / "Source/Shader").toString(), "/Shader/");
	ResourceRegistry::Instance().addResourceSearchPath(Directory::ApplicationDirectory().toString(), "/Project/");
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