#include "editor_application.h"
#include <Engine/Application/window.h>
#include "MainWidget/main_widget.h"
#include <Engine/Renderer/FRP/forward_render_pass.h>
#include <Engine/Renderer/renderer.h>
#include <Engine/Resource/resource_path.h>
#include "editor_core.h"
#include <Engine/Resource/resource_registry.h>
#include <LogWidget/log_widget.h>
#include <Engine/Scene/actor.h>
#include <Engine/Resource/static_mesh.h>
#include <Engine/Scene/primitive_component.h>
#include <Engine/Scene/camera_component.h>
#include <Engine/Scene/level.h>

using namespace volucris;

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	ResourceRegistry::Instance().scanResources("/Engine");

	auto config = volucris::Application::Config(argc, argv);
	auto app = std::make_shared<volucris::EditorApplication>(config);
	auto window = std::make_shared<Window>();
	window->setTitle("Volucris Editor");
	window->setFullScreen(true);
	app->setWindow(window);
	auto widget = std::make_shared<MainWidget>();
	auto logWidget = std::make_shared<LogWidget>();
	widget->setLogWidget(logWidget);
	app->setMainWidget(widget);

	auto renderer = std::make_shared<Renderer>();
	app->setRenderer(renderer);

	auto level = std::make_shared<Level>();

	{
		auto mesh = ResourceRegistry::Instance().loadResource<StaticMesh>("/Engine/cube");
		auto comp = std::make_shared<PrimitiveComponent>();
		comp->setMeshResource(mesh->getResource());
		comp->setMaterials(mesh->getMaterials());
		auto actor = std::make_shared<Actor>();
		actor->addComponent(comp);
		level->addActor(actor);
	}

	gApp->setLevel(level);

	return app;
}