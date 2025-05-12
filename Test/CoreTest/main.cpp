#include <Engine/Core/logging.h>
#include <Engine/Core/assert.h>
#include <Engine/Application/application.h>
#include <Engine/Application/window.h>
#include <Engine/Application/viewport.h>
#include <Engine/Application/client_viewport.h>
#include <Engine/Scene/scene.h>
#include "Engine/Scene/actor.h"
#include "Engine/Scene/primitive_component.h"
#include "Engine/Resource/mesh_resource_data.h"
#include "Engine/Resource/resource_path.h"
#include <Engine/Resource/resource_manager.h>

VOLUCRIS_DECLARE_LOG(CoreTest, Trace)

VOLUCRIS_DEFINE_LOG(CoreTest)

using namespace volucris;

glm::vec3 vertices[] =
{
		{-0.5, -0.5, -0.5},
		{ 0.5, -0.5, -0.5},
		{ 0.5,  0.5, -0.5},
		{-0.5,  0.5, -0.5},

		{-0.5, -0.5,  0.5},
		{ 0.5, -0.5,  0.5},
		{ 0.5,  0.5,  0.5},
		{-0.5,  0.5,  0.5},

		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5,  0.5 },
		{ -0.5,  0.5, -0.5 },
		{ -0.5,  0.5, -0.5 },

		{  0.5, -0.5,  0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{  0.5,  0.5,  0.5 },

		//
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
		{-0.5, 0.5, -0.5},
		{-0.5, 0.5, -0.5},

		{-0.5, -0.5, -0.5},
		{-0.5, -0.5, -0.5},
		{0.5, -0.5, 0.5},
		{-0.5, -0.5, 0.5},
};

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	auto config = Application::Config(argc, argv);
	auto app = std::make_shared<Application>(config);

	auto vp = std::make_shared<ClientViewport>();
	auto scene = std::make_shared<Scene>();
	scene->addViewport(vp);

	auto actor = std::make_shared<Actor>();
	auto comp = std::make_shared<PrimitiveComponent>();
	comp->setDisplayName("PrimitiveComp");
	actor->setRootComponent(comp);
	scene->addActor(actor);

	auto data = comp->getMeshResourceData();
	auto mat = gResources->getMaterialFromPath(ResourcePath("/Engine/Content/Material/default_mesh.mat"));
	data->setVertices(&vertices, sizeof(vertices));
	data->setMaterial("", mat);
	auto section = std::make_shared<Section>();
	section->indices = 
	data->addSection()
	comp->markRenderStateDirty();

	app->addScene(scene);

	return app;
}