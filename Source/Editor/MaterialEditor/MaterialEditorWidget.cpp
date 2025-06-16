#include "MaterialEditorWidget.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace volucris
{
	void MaterialEditorWidget::onBuild()
	{
		auto dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGuiIO& io = ImGui::GetIO();

        // 检查是否有保存的布局
        if (ImGui::GetFrameCount() == 1)
        {
            ImGui::DockBuilderRemoveNode(dockspace_id); // 清除现有布局（如果有）
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // 添加新的 DockSpace
 
            // 将 DockSpace 拆分为左右两部分（比例 1:1）
            ImGuiID left_id, right_id;
            ImGui::DockBuilderSplitNode(
                dockspace_id,
                ImGuiDir_Left,    // 方向：左侧
                50.f,             // 左侧占 50%
                &left_id,         // 左侧 DockNode ID
                &right_id         // 右侧 DockNode ID
            );

            ImGui::DockBuilderSetNodeSize(left_id, {600,100});

            // 将窗口绑定到 DockNode
            ImGui::DockBuilderDockWindow("Preview", left_id);
            ImGui::DockBuilderDockWindow("Editor", right_id);

            // 完成布局设置
            ImGui::DockBuilderFinish(dockspace_id);
        }

        static ImGuiWindowClass no_title_class;
        no_title_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton |
            ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_HiddenTabBar |
            ImGuiDockNodeFlags_NoWindowMenuButton;

        // 对每个窗口
        ImGui::SetNextWindowClass(&no_title_class);

		// 窗口 2-4：普通 ImGui 控件窗口
		ImGui::Begin("Preview", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("This is a control panel.");
		ImGui::End();

        ImGui::SetNextWindowClass(&no_title_class);
		ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("This is a material editor widget.");
		ImGui::End();
	}
} // namespace volucris