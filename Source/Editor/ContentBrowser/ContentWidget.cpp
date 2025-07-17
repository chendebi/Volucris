#include "ContentWidget.h"
#include <Engine/FileSystem/FileSystem.h>
#include <ContentBrowser/ContentItemWidget.h>
#include <EditorEntry/EditorApplication.h>
#include <Engine/Asset/AssetManager.h>
#include <Engine/Game/Texture2D.h>
#include <imgui.h>
#include <EditorEntry/EditorWindow.h>

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
		m_controlItem = nullptr;
		RHITexture2D* iconTexture = nullptr;
		if (auto window = dynamic_cast<EditorWindow*>(getTopWidget()))
		{
			iconTexture = window->getEditorIconTexture();
		}
		auto nodes = gFileSystem.getFileNodes(folder);
		m_items.clear();
		for (auto node : nodes)
		{
			if (node.type == EFileType::Directory)
			{
				auto item = std::make_unique<ContentItemWidget>(node);
				item->setIcon({ 0, 0 }, { 128,128 });
				item->setScale(m_scale);
				item->setTexture(iconTexture);
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
				m_items.emplace_back(std::move(item));
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
}
