#include "EditorApplication.h"
#include <Engine/Application/Window.h>
#include <Engine/Render/ImGuiRenderer.h>
#include <MaterialEditor/MaterialEditorWidget.h>
#include <imgui/imgui.h>
#include <Engine/FileSystem/FileSystem.h>
#include <Engine/RHI/RHICommandList.h>
#include <Engine/RHI/RHITexture.h>
#include <EditorCore/Editor.h>
#include <EditorCore/ImageLoader.h>
#include <imgui_internal.h>
#include <ContentBrowser/ContentItemWidget.h>

#include <AssetEditor/ContentBrowerWidget.h>

namespace volucris
{
	EditorApplication::EditorApplication()
		: Application()
	{
	}
}

using namespace volucris;

static bool ContentItemButton(RHITexture2D* texture, Point pos, const char* label)
{
	static float uvSize = 64.0f / 2048.0f;

	ImVec2 uv_min = ImVec2(pos.x * uvSize, 1.0f - pos.y * uvSize); // 左上角UV坐标
	ImVec2 uv_max = ImVec2(uv_min.x + uvSize, uv_min.y - uvSize); // 右下角UV坐标

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImVec2 button_size(96, 128);
	ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

	// 1. 创建按钮点击区域
	ImGui::InvisibleButton("##xx", button_size);
	bool clicked = ImGui::IsItemClicked();
	bool hovered = ImGui::IsItemHovered();

	// 2. 绘制背景（整个按钮区域）
	ImU32 bg_color = IM_COL32(0, 0, 0, 255);
	window->DrawList->AddRectFilled(cursor_pos,
		ImVec2(cursor_pos.x + button_size.x, cursor_pos.y + button_size.y),
		bg_color);

	auto id = texture->getId();
	ImTextureID texID = (ImTextureID)(intptr_t)id;
	ImVec2 imgPos = { cursor_pos.x + 4, cursor_pos.y+4 };
	/*window->DrawList->AddImage(texID,
		imgPos,
		ImVec2(imgPos.x + 92, imgPos.y + 92),
		uv_min, uv_max);*/
	bg_color = IM_COL32(0, 128, 200, 255);
	window->DrawList->AddRectFilled(imgPos,
		ImVec2(imgPos.x + 88, imgPos.y + 88),
		bg_color);

	ImVec2 text_pos(cursor_pos.x + 32, cursor_pos.y + 80); // 垂直居中
	ImU32 text_color = IM_COL32(255, 255, 255, 255);

	// 文字居中处理
	ImVec2 text_size = ImGui::CalcTextSize(label);
	text_pos.x -= text_size.x * 0.5f;

	window->DrawList->AddText(text_pos, text_color, label);

	if (hovered) {
		window->DrawList->AddRect(cursor_pos,
			ImVec2(cursor_pos.x + button_size.x, cursor_pos.y + button_size.y),
			IM_COL32(200, 200, 200, 255));
	}
}

class MWidget : public Widget
{
public:
	MWidget() : Widget(), m_iconTexture(nullptr) 
	{
		
	}

	void onBuild() override
	{
		int width = 96 * 1.;
		int space = 10;
		ImGui::Begin("Grid Layout");
		auto size = ImGui::GetContentRegionAvail();
		int num_columns = (size.x + space) / (width + space);  // 列数
		ImGui::Columns(num_columns, nullptr, false); // 创建列
		
		for (int i = 0; i < 9; ++i) {
			ImGui::PushID(i);
			m_items[i % 3].build();
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1); // 结束列
		ImGui::End();
	}

private:
	void onTopWidgetChanged(Widget* old, Widget* current) override
	{
		if (old)
		{
			if (auto window = dynamic_cast<Window*>(old))
			{
				window->AttachStateChanged.unbind(this);

				if (m_iconTexture)
				{
					onWindowAttachStateChanged(window, false);
				}
			}
		}

		auto window = dynamic_cast<Window*>(current);
		if (!window)
		{
			return;
		}
		window->AttachStateChanged.bindObject(this, &MWidget::onWindowAttachStateChanged);
		if (window->getImGuiRenderer())
		{
			onWindowAttachStateChanged(window, true);
		}
	}

	void onWindowAttachStateChanged(Window* window, bool attached)
	{
		if (attached)
		{
			auto cmdList = window->getImGuiRenderer()->getCommandList();
			loadIcons(cmdList);
		}
		else
		{
			window->getImGuiRenderer()->getCommandList()->deleteResource(m_iconTexture.get());
			m_iconTexture = nullptr;
		}
	}

	void loadIcons(RHICommandList* cmdList)
	{
		if (m_iconTexture)
		{
			cmdList->deleteResource(m_iconTexture.get());
			m_iconTexture = nullptr;
		}

		std::string filepath = gFileSystem.virtualToPhysical("/Engine/Resource/Images/icons.png");
		V_LOG_DEBUG(Editor, "icon load file path: {}", filepath);
		ImageLoader loader = ImageLoader(filepath);
		if (loader.load())
		{
			const auto& textureData = loader.getTextureData();
			RHITextureDesc desc;
			desc.pixelFormat = Texture::getPixelFormat(textureData.format);
			desc.size = textureData.size;
			desc.texClass = RHITextureDesc::Texture2D;
			m_iconTexture = std::make_unique<RHITexture2D>(desc);
			cmdList->bindResource(m_iconTexture.get());
			m_iconTexture->init(cmdList);
			m_iconTexture->update(cmdList, { 0, 0, textureData.size.width, textureData.size.height }, textureData.data);

			m_items[0].setIcon(m_iconTexture.get(), {0,0}, {128,128});
			m_items[1].setIcon(m_iconTexture.get(), { 0,0 }, { 128,128 });
			m_items[2].setIcon(m_iconTexture.get(), { 0,0 }, { 128,128 });

			m_items[0].setScale(1.);
			m_items[1].setScale(1.);
			m_items[2].setScale(1.);
		}
	}

private:
	std::unique_ptr<RHITexture2D> m_iconTexture;
	ContentItemWidget m_items[3];
};

std::shared_ptr<volucris::Application> volucrisEntry(int argc, char* argv[])
{
	// 仅在测试时使用
	gFileSystem.mount("/Engine/Resource", fmt::format("{}/Resource", VOLUCRIS_ENGINE_ROOT));
	
	auto app = std::make_shared<EditorApplication>();
	auto window1 = std::make_shared<Window>();
	auto window2 = std::make_shared<Window>();
	window1->setTitle("Volucris Editor");
	window1->addChild(std::make_shared<MWidget>());
	window2->setTitle("Material Editor");
	window1->addChild(std::make_shared<ContentBrowserWidget>());
	window2->addChild(std::make_shared<MaterialEditorWidget>());
	app->addWindow(window1);
	app->addWindow(window2);
	return app;
}