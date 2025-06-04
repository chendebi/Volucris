#ifndef __volucris_asset_list_widget_h__
#define __volucris_asset_list_widget_h__

#include <Engine/Application/widget.h>
#include <string>
#include <glm/glm.hpp>
#include "content_browser_events.h"

namespace volucris
{
	class DirectoryItem;

	class AssetListWidget : public Widget
	{
	public:
		AssetListWidget();

		~AssetListWidget();

		void build() override;

		void setCurrentFiles();

		void renderAssetGrid();

		void setCurrentPath(const std::string& path) {
			m_currentPath = path;
			setCurrentFiles();
		}

		PathSelectedEvent& getDirectorySelectedEvent() { return m_pathSelectedEvent; }

		
	private:

		std::vector<std::string> m_currentFiles;  // 当前显示的文件列表

		float m_thumbnailSize;// 缩略图大小
		glm::vec4 m_assetColor; // 资产框颜色

		std::string m_currentPath;

		PathSelectedEvent m_pathSelectedEvent; 
	};
}

#endif // !__volucris_asset_list_widget_h__
