#ifndef __volucris_path_tree_widget_h__
#define __volucris_path_tree_widget_h__

#include <string>
#include <Engine/Application/widget.h>
#include <glm/glm.hpp>

#include "ContentBrowerWidget.h"
#include "ContentBrowserEvents.h"

namespace volucris
{
	class PathTreeWidget;
	class AddResourceDialog;
	class AssetListWidget;

	enum class ContextMenuAction {
		None,
		New,
		Delete,
		Rename
	};

	class DirectoryItem : public Widget
	{
	public:
		DirectoryItem(PathTreeWidget* manager, const std::string& fullPath);

		void onBuild() override;

		void setDisplayName(const std::string& name)
		{
			m_displayName = name;
		}

		std::string getDisplayName() const
		{
			return m_displayName;
		}

		void setSelected(bool selected)
		{
			m_selected = selected;
		}

		void refresh();

		void addChild(const std::string& dirName);

		std::string getResourceDirectory() const;

		std::string getDirectoryPath() { return m_directoryPath; }

		void setDirectoryPath(std::string name) { m_directoryPath = name; }

		std::vector<DirectoryItem> getChild() { return m_subItems; }

	protected:
		bool buildItem();

	private:
		bool m_selected;
		bool m_newPopOpened;
		bool m_isRightClick;
		std::string m_directoryPath;
		std::string m_displayName;
		PathTreeWidget* m_manager;
		std::vector<DirectoryItem> m_subItems;
	};

	//DECLARE_EVENT_DELEGATE(OnPathSelectedEvent, void, std::string)

	class PathTreeWidget : Widget
	{
	public:
		//OnPathSelectedEvent pathSelectedEvent;

	public:
		PathTreeWidget();

		void addRootPath(const std::string& rootPath, const std::string& displayName);

		void onBuild() override;

		void setWidth(float width) { m_width = width; }

		void connectToDirectoryEvent(OnPathSelectedEvent& event) {
			m_pathSelectedEvent = &event;
		}

		float getWidth() const { return m_width; }

		void setSelectedItem(DirectoryItem* item);

		void showPopWindow(bool show) { m_popOpened = show; }

		void showActionDialog(DirectoryItem* node);

		void showConfirmDialogFunc(DirectoryItem* node);

		void performFileSystemAction(DirectoryItem* node);

		DirectoryItem* getSelectedPathItem() const { return m_selectedItem; }

		DirectoryItem* findParent(DirectoryItem* root, DirectoryItem* target);

		OnPathSelectedEvent* getSelectedEvent() { return m_pathSelectedEvent; }

	protected:


	private:
		float m_width;
		bool m_showAddResourceWidget;
		bool m_popOpened;
		bool m_pendingNewDialog;
		bool m_showActionDialog;
		bool m_showConfirmDialog;
		glm::vec2 m_dialogPos;  // 对话框初始位置
		std::vector<DirectoryItem> m_rootItems;
		DirectoryItem* m_selectedItem;
		ContextMenuAction m_currentAction;
		char m_inputBuffer[128] = "";
		std::shared_ptr<AddResourceDialog> m_addResourceDlg;

		OnPathSelectedEvent* m_pathSelectedEvent = nullptr;

	};
}

#endif // !__volucris_path_tree_widget_h__
