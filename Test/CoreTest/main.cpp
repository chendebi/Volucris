#include <Engine/Core/logging.h>
#include <Engine/Core/assert.h>
#include <Engine/Application/application.h>
#include <Engine/Application/window.h>
#include <Engine/Scene/viewport.h>
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
#include "Engine/Scene/primitive_component.h"
#include <Engine/Scene/camera_component.h>

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
		8, 9, 10, 9, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
};

class MyApplication : public volucris::Application
{
public:
	MyApplication(const Config& config)
		: Application(config)
	{ }

	void setPrimitiveComp(std::shared_ptr<PrimitiveComponent> comp)
	{
		m_primitive = comp;
	}

protected:
	void tick(double delta) override
	{
		x += (m_dir * delta * 10.0);
		if (x > 1.0)
		{
			m_dir = -0.1;
		}
		else if (x < -1.0)
		{
			m_dir = 0.1;
		}
		m_primitive->setPosition({ x, 0.0, 0.0 });

	}

private:
	float x = 0.0;
	float m_dir = 0.1;
	std::shared_ptr<PrimitiveComponent> m_primitive;
};

std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[])
{
	auto config = Application::Config(argc, argv);
	auto app = std::make_shared<MyApplication>(config);

	auto vp = std::make_shared<ClientViewport>();
	auto scene = std::make_shared<Scene>();
	scene->addViewport(vp);

	auto actor = std::make_shared<Actor>();
	auto comp = std::make_shared<PrimitiveComponent>();
	comp->setDisplayName("PrimitiveComp");
	actor->setRootComponent(comp);
	scene->addActor(actor);
	app->setPrimitiveComp(comp);

	auto data = comp->getMeshResourceData();
	auto mat = gResources->getMaterialFromPath(ResourcePath("/Engine/Content/Material/default_mesh.mat"));
	mat->getParameterByName("fcolor")->setValue(glm::vec3(0.1, 1.0, 0.0));
	data->setVertices(&vertices, sizeof(vertices));
	data->setMaterial("", mat);
	auto section = data->addSectionData(indices, sizeof(indices) / sizeof(uint32));
	section.mode = DrawMode::TRIANGLES;
	section.slot = "";
	section.offset = 12 * sizeof(uint32);
	section.count = 6;
	data->addSection(section);
	comp->markRenderStateDirty();

	app->addScene(scene);

	auto camera = std::make_shared<CameraComponent>(CameraComponent::PERSPECTIVE, vp.get());
	auto cameraActor = std::make_shared<Actor>();
	cameraActor->setRootComponent(camera);
	scene->addActor(cameraActor);
	camera->setPosition({ 0, 3, 1 });
	camera->setRotation({-60.0, 0.0, 0.0});

	auto renderer = std::make_shared<Renderer>();
	renderer->addRenderPass(std::make_shared<SimplePass>());
	app->setRenderer(renderer);
	return app;
}