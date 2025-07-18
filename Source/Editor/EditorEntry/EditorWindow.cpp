#include "EditorWindow.h"
#include <EditorCore/editor.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <MaterialEditor/MaterialEditorWidget.h>
#include <Viewport/ViewportWidget.h>
#include <Engine/Game/Universe.h>
#include <EditorEntry/LogWidget.h>
#include <ContentBrowser/ContentBrowserWidget.h>
#include <Engine/RHI/RHITexture.h>
#include <Engine/Asset/AssetManager.h>
#include <Engine/Game/Texture2D.h>
#include "EditorApplication.h"

#include <stb_image/stb_image_write.h>
#include <Engine/FileSystem/FileSystem.h>
#include <fstream>
#include <EditorCore/ImageLoader.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <filesystem>
#include <Engine/Core/Assert.h>
#include <iostream>
#include <Engine/Game/StaticMesh.h>

namespace fs = std::filesystem;

namespace volucris
{
    MainWidget::MainWidget()
        : Widget()
        , m_viewport(std::make_shared<ViewportWidget>())
        , m_logWidget(std::make_shared<LogWidget>())
        , m_contentBrowserWidget(std::make_shared<ContentBrowserWidget>())
    {
        addChild(m_viewport);
        addChild(m_logWidget);
        addChild(m_contentBrowserWidget);
        m_logWidget->init();
        m_viewport->setUniverse(std::make_shared<Universe>());
    }

    void MainWidget::onBuild(bool)
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

        bool init = ImGui::GetFrameCount() == 1 && !hasCustomLayout("MainWindow");
        if (init)
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
            ImGui::PushID(0);
            ImGui::DockBuilderDockWindow("Content Browser", bottomId);
            ImGui::PopID();

            // 完成布局设置
            ImGui::DockBuilderFinish(dockspace);
        }
        //ImGui::SetNextWindowDockID(dockspace);
        static ImGuiWindowClass no_title_class;
        no_title_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        // 对每个窗口
       ImGui::SetNextWindowClass(&no_title_class);
        m_viewport->build(init);

        m_logWidget->build(init);

        m_contentBrowserWidget->build(init);

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
        , m_iconTexture(nullptr)
	{
		
	}

    RHITexture2D* EditorWindow::getEditorIconTexture() const
    {
        return m_iconTexture.get();
    }

	void EditorWindow::onRendererBuild(RHICommandList* cmdList)
	{
		ImGuiIO& io = ImGui::GetIO();

        if (auto texture = AssetManager::getInstance().loadAsset<Texture2D>("/Engine/Content/Editor/Textures/T_Icons", GEditorWorld))
        {
            const auto& data = texture->getTextureData();
            RHITextureDesc desc;
            desc.size = data.size;
            desc.sourceFormat = data.format;
            desc.pixelFormat = Texture::EPixelFormat::R8G8B8A8;
            desc.texClass = TextureType::Texture2D;
            m_iconTexture = std::make_unique<RHITexture2D>(desc);
            m_iconTexture->setContext(cmdList);
            m_iconTexture->createGpuResource();
            m_iconTexture->init(data.data);
        }

        auto mesh = AssetManager::getInstance().loadAsset<StaticMesh>("/Engine/Content/Editor/Cube", GEditorWorld);
	}

    void EditorWindow::onRendererDestroy(RHICommandList* cmdList)
    {
        m_iconTexture = nullptr;
    }
}