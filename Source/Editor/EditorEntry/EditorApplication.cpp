#include "EditorApplication.h"
#include <Engine/Application/Window.h>
#include <Engine/Render/ImGuiRenderer.h>
#include <MaterialEditor/MaterialEditorWidget.h>
#include <imgui/imgui.h>
#include <Engine/FileSystem/FileSystem.h>
#include <EditorEntry/EditorWindow.h>


namespace volucris
{
	EditorWorld::EditorWorld()
		: World()
	{

	}

	EditorApplication::EditorApplication()
		: Application()
		, m_world(std::make_unique<EditorWorld>())
	{
	}
}

using namespace volucris;

std::shared_ptr<volucris::Application> volucrisEntry(int argc, char* argv[])
{
	// 仅在测试时使用
	gFileSystem.mount("/Engine/Resource", fmt::format("{}/Resource", VOLUCRIS_ENGINE_ROOT));
	gFileSystem.mount("/Engine/Shader", fmt::format("{}/Source/Shader", VOLUCRIS_ENGINE_ROOT));
	
	auto app = std::make_shared<EditorApplication>();
	auto window = std::make_shared<EditorWindow>();
	app->addWindow(window);

	window->setTitle("Volucris Editor");
	auto widget = std::make_shared<MainWidget>();
	window->addChild(widget);

	return app;
}