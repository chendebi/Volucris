#include "main_widget.h"
#include <imgui/imgui.h>
#include "ContentBrowser/content_browser_widget.h"
#include <LogWidget/log_widget.h>
#include <PropertyWidget/property_widget.h>

namespace volucris
{
	MainWidget::MainWidget()
		: Widget()
		, m_browserWidget(std::make_shared<ContentBrowserWidget>())
		, m_logWidget(nullptr)
		, m_viewport()
	{
	}

	void MainWidget::build()
	{
		auto id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		ImGui::SetNextWindowDockID(id);
		// 窗口 1：Shader 渲染窗口
		m_viewport.build();

		// 窗口 2-4：普通 ImGui 控件窗口
		ImGui::Begin("Controls");
		ImGui::Text("This is a control panel.");
		ImGui::End();

		if (m_logWidget)
		{
			m_logWidget->build();
		}

		m_propertyWidget->build();
		
		m_browserWidget->build();
	}
}