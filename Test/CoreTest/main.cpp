#include <Engine/Core/logging.h>
#include <Engine/Core/assert.h>
#include <Engine/Application/application.h>
#include <Engine/Application/window.h>
#include <Engine/Application/viewport.h>
#include <Engine/Application/client_viewport.h>
#include <Engine/Scene/scene.h>

VOLUCRIS_DECLARE_LOG(CoreTest, Trace)

VOLUCRIS_DEFINE_LOG(CoreTest)

using namespace volucris;

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	auto config = Application::Config(argc, argv);
	auto app = std::make_shared<Application>(config);

	auto vp = std::make_shared<ClientViewport>();
	auto scene = std::make_shared<Scene>();
	scene->addViewport(vp);
	app->addScene(scene);

	return app;
}