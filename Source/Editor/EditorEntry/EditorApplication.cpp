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
#include <Engine/Game/Texture2D.h>
#include <Engine/Game/Package.h>
#include <Engine/Asset/AssetManager.h>

namespace volucris
{
	EditorApplication::EditorApplication()
		: Application()
	{
	}
}

using namespace volucris;

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
		num_columns = std::max(num_columns, 1); // 确保至少有一列
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
	void onRendererBuild(RHICommandList* context) override
	{
		loadIcons(context);
	}

	void onRendererDestroy(RHICommandList* context) override
	{
		context->deleteResource(m_iconTexture.get());
		m_iconTexture = nullptr;
	}

	void loadIcons(RHICommandList* cmdList) 
	{
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

	const std::string iconPath = "/Engine/Content/Editor/Textures/T_Icons";
	std::string filepath = gFileSystem.virtualToPhysical("/Engine/Resource/Images/icons.png");
	V_LOG_DEBUG(Editor, "icon load file path: {}", filepath);
	ImageLoader loader = ImageLoader(filepath);
	if (loader.load())
	{
		const auto& textureData = loader.getTextureData();
		auto package = std::make_shared<Package>();
		auto texture2d = std::make_shared<Texture2D>(textureData);
		texture2d->setParent(package.get());

		AssetManager::getInstance().registry(package.get());
		AssetManager::getInstance().save(package.get());
		//texture2d = nullptr;
	}
	/*Texture2D t;
	auto package = AssetManager::getInstance().load(iconPath);*/

	return app;
}