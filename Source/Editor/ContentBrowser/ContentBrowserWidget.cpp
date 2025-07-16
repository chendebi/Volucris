#include <ContentBrowser/ContentBrowserWidget.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ContentBrowser/ContentWidget.h>

namespace volucris
{
    ContentBrowserWidget::ContentBrowserWidget()
        : Widget()
        , m_contentWidget(std::make_shared<ContentWidget>())
    {
        addChild(m_contentWidget);
    }

	void ContentBrowserWidget::onBuild(bool init)
	{
        static char* WindowName = "Content Browser";
        //ImGui::PushID(0);
        ImGui::Begin(WindowName);
        ImGuiID dockspace = ImGui::GetID(WindowName);
        ImGui::DockSpace(dockspace);

        if (init)
        {
            ImGui::DockBuilderRemoveNode(dockspace); // 清除现有布局（如果有）
            ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace); // 添加新的 DockSpace


            ImGuiID left_id, right_id;
            ImGui::DockBuilderSplitNode(
                dockspace,
                ImGuiDir_Left,
                .6f,
                &left_id,
                &right_id
            );

            // 将窗口绑定到 DockNode
            ImGui::DockBuilderDockWindow("Navgation", left_id);
            ImGui::DockBuilderDockWindow("Content", right_id);

            // 完成布局设置
            ImGui::DockBuilderFinish(dockspace);
        }

        static ImGuiWindowClass no_title_class;
        no_title_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        // 对每个窗口
        ImGui::SetNextWindowClass(&no_title_class);
        ImGui::Begin("Navgation");
        ImGui::End();

        ImGui::SetNextWindowClass(&no_title_class);
        m_contentWidget->build();

        //ImGui::PopID();
        ImGui::End();
	}
}