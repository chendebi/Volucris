#include "main_widget.h"
#include <imgui/imgui.h>
#include "ContentBrowser/content_browser_widget.h"

namespace volucris
{
	MainWidget::MainWidget()
		: Widget()
		, m_browserWidget(std::make_shared<ContentBrowserWidget>())
	{
	}

	void MainWidget::build()
	{
		auto id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		ImGui::SetNextWindowDockID(id);
		// ���� 1��Shader ��Ⱦ����
		ImGui::Begin("Shader View");
		// ���������� Shader ��Ⱦ���루������ 3��
		ImGui::End();

		// ���� 2-4����ͨ ImGui �ؼ�����
		ImGui::Begin("Controls");
		ImGui::Text("This is a control panel.");
		ImGui::End();

		ImGui::Begin("Log");
		ImGui::Text("Log messages...");
		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::SliderFloat("Value", &m_value, 0.0f, 1.0f);
		ImGui::End();

		m_browserWidget->build();
	}
}