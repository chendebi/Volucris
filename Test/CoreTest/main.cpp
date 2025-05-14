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
#include <Engine/Renderer/renderer.h>
#include "Engine/Resource/material.h"
#include "Engine/Resource/material_parameter.h"
#include "simple_pass.h"

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

unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
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
	mat->getParameterByName("fcolor")->setValue(glm::vec3(0.1, 1.0, 0.0));
	data->setVertices(&vertices, sizeof(vertices));
	data->setMaterial("", mat);
	auto section = data->addSectionData(indices, sizeof(indices) / sizeof(uint32));
	section.mode = DrawMode::TRIANGLES;
	section.slot = "";
	data->addSection(section);
	comp->markRenderStateDirty();

	app->addScene(scene);

	auto renderer = std::make_shared<Renderer>();
	renderer->addRenderPass(std::make_shared<SimplePass>());
	app->setRenderer(renderer);
	return app;
}