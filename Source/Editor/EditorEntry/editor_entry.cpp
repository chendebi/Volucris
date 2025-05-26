#include "editor_application.h"
#include <Engine/Application/window.h>
#include "MainWidget/main_widget.h"
#include <Engine/Renderer/FRP/forward_render_pass.h>
#include <Engine/Renderer/renderer.h>
#include <Engine/Resource/resource_path.h>
#include "editor_core.h"
#include <Engine/Resource/resource_registry.h>

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

	auto renderer = std::make_shared<Renderer>();
	renderer->addRenderPass(std::make_shared<ForwardRenderPass>());
	app->setRenderer(renderer);

	auto meta = ResourceRegistry::Instance().getResourceMeta("/Engine/M_SS");

	return app;
}