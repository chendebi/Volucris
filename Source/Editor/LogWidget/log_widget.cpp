#include "log_widget.h"
#include <imgui.h>
#include <fmt/format.h>
#include <Engine/Core/logger.h>

namespace volucris
{
	LogWidget::LogWidget()
		: Widget()
	{

	}

	void LogWidget::build()
	{
		ImGui::Begin("Log");
		
		ImGui::BeginChild("LogContent");
		
		ImGuiListClipper clipper;
		clipper.Begin(gLogBuffer.count());
		while (clipper.Step()) {
			std::string msg;
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
				gLogBuffer.get(i, msg);
				if (msg.find("[error]") != std::string::npos) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); // 红色
				}
				else if (msg.find("[warning]") != std::string::npos) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); // 黄色
				}
				else if (msg.find("[info]") != std::string::npos)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 200, 0, 255)); // 黄色
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255)); // 黄色
				}
				ImGui::TextUnformatted(msg.c_str());
				ImGui::PopStyleColor();
			}
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();

		ImGui::End();
	}
}
