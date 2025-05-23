#ifndef __volucris_path_tree_widget_h__
#define __volucris_path_tree_widget_h__

#include <string>
#include <Engine/Application/widget.h>

namespace volucris
{
	class PathTreeWidget;
	class AddResourceDialog;

	class DirectoryItem : public Widget
	{
	public:
		DirectoryItem(PathTreeWidget* manager, const std::string& fullPath);

		void build() override;

		void setDisplayName(const std::string& name)
		{
			m_displayName = name;
		}

		void setSelected(bool selected)
		{
			m_selected = selected;
		}

		std::string getResourceDirectory() const;

	protected:
		bool buildItem();

	private:
		bool m_selected;
		std::string m_directoryPath;
		std::string m_displayName;
		PathTreeWidget* m_manager;
		std::vector<DirectoryItem> m_subItems;
	};

	class PathTreeWidget : Widget
	{
	public:
		PathTreeWidget();

		void addRootPath(const std::string& rootPath, const std::string& displayName);

		void build() override;

		void setWidth(float width) { m_width = width; }

		float getWidth() const { return m_width; }

		void setSelectedItem(DirectoryItem* item);

		DirectoryItem* getSelectedPathItem() const { return m_selectedItem; }

	protected:
		

	private:
		float m_width;
		bool m_showAddResourceWidget;
		std::vector<DirectoryItem> m_rootItems;
		DirectoryItem* m_selectedItem;
		std::shared_ptr<AddResourceDialog> m_addResourceDlg;
	};
}

#endif // !__volucris_path_tree_widget_h__
