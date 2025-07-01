#include "EditorApplication.h"
#include <Engine/Application/Window.h>
#include <Engine/Render/ImGuiRenderer.h>
#include <MaterialEditor/MaterialEditorWidget.h>
#include <imgui/imgui.h>

#include <AssetEditor/ContentBrowerWidget.h>

namespace volucris
{
	EditorApplication::EditorApplication()
	{
	}
}

using namespace volucris;

class MWidget : public Widget
{
public:

	void onBuild() override
	{
		ImGui::Begin("Content Browser");
		ImGui::Text("Hello Imgui");
		ImGui::End();
	}
};

std::shared_ptr<volucris::Application> volucrisEntry(int argc, char* argv[])
{
	auto app = std::make_shared<EditorApplication>();
	auto window1 = std::make_shared<Window>();
	auto window2 = std::make_shared<Window>();
	window1->setTitle("Volucris Editor");
	window1->addChild(std::make_shared<MWidget>());
	window2->setTitle("Material Editor");
	window1->addChild(std::make_shared<ContentBrowserWidget>());
	window2->addChild(std::make_shared<MaterialEditorWidget>());
	//app->addWindow(window1);
	app->addWindow(window2);
	return app;
}