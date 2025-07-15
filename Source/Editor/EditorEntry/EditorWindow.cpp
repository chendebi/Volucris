#include "EditorWindow.h"
#include <EditorCore/editor.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <MaterialEditor/MaterialEditorWidget.h>
#include <Viewport/ViewportWidget.h>
#include <Engine/Game/Universe.h>
#include <EditorEntry/LogWidget.h>

namespace volucris
{
    MainWidget::MainWidget()
        : Widget()
        , m_viewport(std::make_shared<ViewportWidget>())
        , m_logWidget(std::make_shared<LogWidget>())
    {
        addChild(m_viewport);
        addChild(m_logWidget);
        m_viewport->setUniverse(std::make_shared<Universe>());
    }

    void MainWidget::onBuild()
	{
        // 创建主窗口（包含DockSpace和菜单栏）
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        // 窗口样式设置（无边框、无标题栏）
        static ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar |
            ImGuiDockNodeFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;

        // 开始主窗口
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MainWindow", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        buildMenubar();

        ImGuiID dockspace = ImGui::GetID("MainWindow");
        ImGui::DockSpace(dockspace);

        if (ImGui::GetFrameCount() == 1 && !hasCustomLayout("MainWindow"))
        {
            ImGui::DockBuilderRemoveNode(dockspace); // 清除现有布局（如果有）
            ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace); // 添加新的 DockSpace

            // 拆分为上下部分
            ImGuiID topId, bottomId;
            ImGui::DockBuilderSplitNode(
                dockspace,
                ImGuiDir_Down,
                .4f,         
                &bottomId,
                &topId
            );


            ImGuiID left_id, right_id;
            ImGui::DockBuilderSplitNode(
                topId,
                ImGuiDir_Right,
                .4f,
                &right_id,
                &left_id
            );

            ImGui::DockBuilderSetNodeSize(bottomId, { 1.0f, viewport->WorkSize.y * 0.4f});
            ImGui::DockBuilderSetNodeSize(right_id, { viewport->WorkSize.x * 0.2f, 1.0f});

            // 将窗口绑定到 DockNode
            ImGui::DockBuilderDockWindow("Preview", left_id);
            ImGui::DockBuilderDockWindow("Property", right_id);
            ImGui::DockBuilderDockWindow("Log", bottomId);
            ImGui::DockBuilderDockWindow("Content", bottomId);

            // 完成布局设置
            ImGui::DockBuilderFinish(dockspace);
        }
        //ImGui::SetNextWindowDockID(dockspace);
        static ImGuiWindowClass no_title_class;
        no_title_class.DockNodeFlagsOverrideSet = 
            ImGuiDockNodeFlags_NoTabBar;

        // 对每个窗口
       ImGui::SetNextWindowClass(&no_title_class);
        m_viewport->build();

        m_logWidget->build();

        ImGui::Begin("Content");
        ImGui::End();

        //ImGui::SetNextWindowDockID(dockspace);
        ImGui::Begin("Property");
        ImGui::End();

        // 结束主窗口
        ImGui::End();
	}

    void MainWidget::buildMenubar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) { /* 操作逻辑 */ }
                if (ImGui::MenuItem("Open")) { /* 操作逻辑 */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) { /* 退出逻辑 */ }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                //if (ImGui::MenuItem("撤销", "Ctrl+Z")) { /* 操作逻辑 */ }
                //if (ImGui::MenuItem("重做", "Ctrl+Y", false, false)) {} // 禁用状态
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Layout"))
            {
                if (ImGui::MenuItem("Save")) {
                    ImGuiIO& io = ImGui::GetIO();
                    ImGui::SaveIniSettingsToDisk(io.IniFilename);
                }
                // 重置布局
                if (ImGui::MenuItem("Reset")) 
                {

                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

	EditorWindow::EditorWindow()
		: Window()
	{
		setTitle("Volucris Editor");
		m_widget = std::make_shared<MainWidget>();
		m_widget->setParent(this);
	}

	void EditorWindow::onRendererBuild(RHICommandList* cmdList)
	{
		ImGuiIO& io = ImGui::GetIO();
		//io.IniFilename = "imgui_layout.ini";
		//V_LOG_DEBUG(Editor, "loaded: {}", io.)
	}

    void EditorWindow::onRendererDestroy(RHICommandList* cmdList)
    {
    }
}