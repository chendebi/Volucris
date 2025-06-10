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
#include <Engine/Resource/material.h>
#include <Engine/Scene/direction_light_component.h>
#include <ContentBrowser/material_loader.h>
#include <Engine/Scene/static_mesh_component.h>
#include <ContentBrowser/mesh_loader.h>

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

	if (true)
	{
		/*auto mesh = ResourceRegistry::Instance().loadResource<StaticMesh>("/Engine/Cube");
		auto actor = level->addActor<StaticMeshComponent>(mesh);*/

		MaterialLoader loader;
		std::string vsf, fsf;
		ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader/default_mesh.vert", vsf);
		ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader/default_mesh.frag", fsf);
		auto resource = loader.load(vsf, fsf);
		auto material = std::make_shared<Material>(resource);

		ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader/default_mesh.frag", fsf);
		MeshLoader meshLoader = MeshLoader("");
		meshLoader.load("D:\\Projects\\Volucris\\Assets\\simple_cube.fbx");
		auto meshes = meshLoader.getLoadedStaticMeshes();
		for (const auto& mesh : meshes)
		{
			for (const auto& slot : mesh->getMaterialSlots())
			{
				mesh->setMaterial(slot, material);
			}
			auto comp = level->addActor<StaticMeshComponent>(mesh);
			comp->setScale({ 100,100,100 });
		}
	}

	gApp->setLevel(level);

	return app;
}