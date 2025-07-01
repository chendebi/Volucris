#include "MaterialEditorWidget.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <Engine/Application/Window.h>
#include <Engine/FileSystem/FileSystem.h>
#include <EditorCore/editor.h>
#include <Viewport/ViewportWidget.h>

namespace volucris
{
    MaterialEditorWidget::MaterialEditorWidget()
        : m_viewport(std::make_shared<ViewportWidget>())
    {
        
    }

    void MaterialEditorWidget::onBuild()
	{
        auto dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        //ImGuiIO& io = ImGui::GetIO();
		if (ImGui::GetFrameCount() == 1 && !gFileSystem.fileExists("/Engine/Config/ImGuiIniSettings.ini"))
		{
            ImGui::DockBuilderRemoveNode(dockspace_id); // 清除现有布局（如果有）
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // 添加新的 DockSpace

            // 将 DockSpace 拆分为左右两部分
            ImGuiID left_id, right_id;
            ImGui::DockBuilderSplitNode(
                dockspace_id,
                ImGuiDir_Left,    // 方向：左侧
                .5f,             // 左侧占 50%
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

        m_viewport->build();

        ImGui::SetNextWindowClass(&no_title_class);
		ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("This is a material editor widget.");
		ImGui::End();
	}

    void MaterialEditorWidget::onTopWidgetChanged(Widget* old, Widget* current)
    {
		m_viewport->onTopWidgetChanged(old, current);
    }

} // namespace volucris