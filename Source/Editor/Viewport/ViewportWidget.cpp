#include <Viewport/ViewportWidget.h>
#include <imgui.h>

namespace volucris
{
	void ViewportWidget::onBuild()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Preview");
		auto pos = ImGui::GetWindowPos();
		auto winSize = ImGui::GetWindowSize();
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::End();
		ImGui::PopStyleVar();
	}
}