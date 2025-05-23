#include "editor_application.h"
#include <Engine/Application/window.h>
#include "MainWidget/main_widget.h"

#include <Engine/Resource/resource_path.h>
#include "editor_core.h"

using namespace volucris;

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	auto config = volucris::Application::Config(argc, argv);
	auto app = std::make_shared<volucris::EditorApplication>(config);
	auto window = std::make_shared<Window>();
	window->setTitle("Volucris Editor");
	window->setFullScreen(true);
	app->setWindow(window);
	app->setMainWidget(std::make_shared<MainWidget>());

	auto testPath = "/Engine/mat/xxx.json";
	ResourcePath xx(testPath);
	V_LOG_DEBUG(Editor, "full path: {}", xx.getSystemPath());

	return app;
}