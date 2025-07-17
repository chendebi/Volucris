#include "ContentWidget.h"
#include <Engine/FileSystem/FileSystem.h>
#include <ContentBrowser/ContentItemWidget.h>
#include <EditorEntry/EditorApplication.h>
#include <Engine/Asset/AssetManager.h>
#include <Engine/Game/Texture2D.h>
#include <imgui.h>
#include <EditorEntry/EditorWindow.h>
#include <Engine/Application/Event.h>
#include <filesystem>
#include <EditorCore/Editor.h>
#include <EditorCore/ImageLoader.h>
#include <Engine/Game/Package.h>
#include "MeshLoader.h"

namespace fs = std::filesystem;

namespace volucris
{
	ContentWidget::ContentWidget()
		: Widget()
		, m_scale(1.0)
		, m_itemSize(ContentItemWidget::getItemSize())
		, m_multiSelect(false)
		, m_controlItem(nullptr)
	{
		setCurrentFolder("/Engine/Content");
	}

	void ContentWidget::setCurrentFolder(const std::string& folder)
	{
		m_folder = folder;
		m_controlItem = nullptr;
		RHITexture2D* iconTexture = nullptr;
		if (auto window = dynamic_cast<EditorWindow*>(getTopWidget()))
		{
			iconTexture = window->getEditorIconTexture();
		}

		auto createNode = [this, iconTexture](const FileNode& node, const std::string& displayName = "")->std::unique_ptr<ContentItemWidget> {
			auto item = std::make_unique<ContentItemWidget>(node);
			item->setIcon({ 0, 0 }, { 128,128 });
			item->setScale(m_scale);
			item->setTexture(iconTexture);
			if (!displayName.empty())
			{
				item->setDisplayName(displayName);
			}
			item->Clicked.bind([this](ContentItemWidget* clicked) {
				if (!m_multiSelect)
				{
					for (auto& item : m_items)
					{
						if (item.get() != clicked)
						{
							item->setSelected(false);
						}
					}
				}
				});
			item->DoubleClicked.bind([this](ContentItemWidget* clicked) {
				m_controlItem = clicked;
				});
			return item;
			};

		m_items.clear();
		{
			auto parentNode = gFileSystem.parentNode(folder);
			if (!parentNode.path.empty())
			{
				m_items.emplace_back(createNode(parentNode, ".."));
			}
		}

		auto nodes = gFileSystem.getFileNodes(folder);
		for (const auto& node : nodes)
		{
			if (node.type == EFileType::Directory)
			{
				m_items.emplace_back(createNode(node));
			}
		}
	}

	void ContentWidget::onBuild(bool init)
	{
		ImGui::Begin("Content");
		m_multiSelect = ImGui::GetIO().KeyCtrl;
		int width = m_itemSize.x;
		int space = 10 * m_scale;
		auto size = ImGui::GetContentRegionAvail();
		int num_columns = (size.x + space) / (width + space);  // 列数
		num_columns = num_columns < 1 ? 1 : num_columns;
		ImGui::Columns(num_columns, nullptr, false); // 创建列

		ContentItemWidget* clickedItem = nullptr;
		ContentItemWidget* selectItem = nullptr;
		for (int i = 0; i < m_items.size(); ++i) {
			ImGui::PushID(i);
			m_items[i]->build();

			if (m_items[i]->isClicked())
			{
				clickedItem = m_items[i].get();
			}
			else if (m_items[i]->isSelected())
			{
				selectItem = m_items[i].get();
			}

			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1); // 结束列
		ImGui::End();

		if (m_controlItem)
		{
			auto node = m_controlItem->getFileNode();
			if (node.type == EFileType::Directory)
			{
				setCurrentFolder(node.path);
			}
		}
	}

	void ContentWidget::onRendererBuild(RHICommandList* cmdList)
	{
		auto window = (EditorWindow*)getTopWidget();
		auto iconTexture = window->getEditorIconTexture();
		for (const auto& item : m_items)
		{
			item->setTexture(iconTexture);
		}
	}

	void ContentWidget::onRendererDestroy(RHICommandList* cmdList)
	{
		for (const auto& item : m_items)
		{
			item->setTexture(nullptr);
		}
	}

	bool ContentWidget::onDrop(DropEvent* event)
	{
		for (const auto& filepath : event->files)
		{
			V_LOG_INFO(Editor, "drop file: {}", filepath);
			auto path = fs::path(filepath);
			const auto ext = path.extension();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
			{
				V_LOG_INFO(Editor, "convert image file");
				ImageLoader loader = ImageLoader(filepath);
				if (loader.load())
				{
					const auto cpath = fs::path(m_folder);
					const auto name = path.stem().generic_string();
					std::string packageName = (cpath / path.stem()).generic_string();
					if (gFileSystem.fileExists(packageName))
					{
						for (size_t i = 1; i < std::numeric_limits<size_t>::max(); ++i)
						{
							packageName = (cpath / fmt::format("_{}", i)).generic_string();
						}
					}
					auto texture = std::make_shared<Texture2D>(loader.getTextureData());
					auto package = std::make_shared<Package>(packageName);
					AssetManager::getInstance().registry(package.get());
					AssetManager::getInstance().save(package.get());
					V_LOG_INFO(Editor, "convert image success, {}", packageName);
				}
			}
			else if (ext == ".obj" || ext == ".fbx")
			{
				V_LOG_INFO(Editor, "convert mesh file");
				MeshLoader loader = MeshLoader(filepath);
				if (loader.load())
				{
					const auto& meshes = loader.getMeshes();
					
				}
			}
		}
		return true;
	}
}
