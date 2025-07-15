#include "LogWidget.h"
#include <Engine/Application/Application.h>
#include <imgui_internal.h>

namespace volucris
{
	LogWidget::LogWidget()
		: Widget()
		, m_logs(1024)
	{
		//gApp->LogAdded.bindObject(this, &LogWidget::onLogAdded);
	}

	void LogWidget::onBuild()
	{
		ImGui::Begin("Log");

		ImGui::BeginChild("LogContent");

		ImGuiListClipper clipper;
		clipper.Begin(m_logs.count());
		while (clipper.Step()) {
			for (auto it = m_logs.begin(); it != m_logs.end(); ++it) {
				auto& msg = *it;
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

	void LogWidget::onLogAdded(const std::string& log)
	{
		m_logs.push(log);
	}
}
