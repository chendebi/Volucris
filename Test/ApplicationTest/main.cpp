#include <Engine/Application/Application.h>
#include <Engine/Application/Window.h>
#include <Engine/Render/ImGuiRenderer.h>

using namespace volucris;

std::shared_ptr<Application> volucrisEntry(int argc, char** argv)
{
	auto app = std::make_shared<Application>();
	auto window1 = std::make_shared<Window>();
	auto window2 = std::make_shared<Window>();
	window2->setTitle("window 2");
	app->addWindow(window1);
	app->addWindow(window2);
	window1->getImGuiRenderer()->color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	window2->getImGuiRenderer()->color = glm::vec4(0.0, 0.0, 1.0, 1.0);
	return app;
}