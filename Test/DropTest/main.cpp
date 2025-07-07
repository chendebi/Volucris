#include <Engine/Application/Application.h>
#include <Engine/Application/Widget.h>
#include <Engine/Application/Window.h>
#include <imgui/imgui.h>
#include <Engine/Core/Logging.h>
#include <Engine/Application/Event.h>

using namespace volucris;

VOLUCRIS_STATIC_LOG(DropTest, Trace)

class DropTestWidget : public Widget
{
	Rect m_rect;

	public:
	DropTestWidget() : Widget()
	{
	}

	void onBuild() override
	{
		ImGui::Begin("Drop Test");
		ImGui::Text("Drag and drop files here.");
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();

		Rect rect;
		rect.x = pos.x;
		rect.y = pos.y;
		rect.width = size.x;
		rect.height = size.y;

		if (m_rect != rect)
		{
			m_rect = rect;
		}

		ImGui::End();
	}

	bool onDrop(DropEvent* event) override
	{
		if (!m_rect.contains(event->pos))
		{
			return false;
		}
		for (const auto& file : event->files)
		{
			V_LOG_INFO(DropTest, "Dropped file: {}", file);
		}
		return true;
	}
};

std::shared_ptr<Application> volucrisEntry(int argc, char* argv[])
{
	auto app = std::make_shared<Application>();
	auto window = std::make_shared<Window>();

	auto widget = std::make_shared<DropTestWidget>();
	window->addChild(widget);
	app->addWindow(window);
	return app;
}