#ifndef __volucris_asset_list_widget_h__
#define __volucris_asset_list_widget_h__

#include <Engine/Application/widget.h>
//#include "Engine/Resource/resource_object.h"
#include <string>
#include <glm/glm.hpp>
#include "ContentBrowserEvents.h"

namespace volucris
{
	class DirectoryItem;

	class AssetListWidget : public Widget
	{
	public:
		AssetListWidget();

		~AssetListWidget();

		void onBuild() override;

		void setCurrentFiles(const std::string& path);

		void renderAssetGrid();

		//void setAssetObject(std::shared_ptr<ResourceObject> res);

		void setCurrentPath(const std::string& path) {
			m_currentPath = path;
			setCurrentFiles(path);
		}

		void drawFileItem(const std::string& filePath);

		void drawObjectItem(/*std::shared_ptr<ResourceObject> obj*/);

		//void handleObjectClick(std::shared_ptr<ResourceObject> obj);

		OnPathSelectedEvent& getDirectorySelectedEvent() { return m_pathSelectedEvent; }


	private:

		std::vector<std::string> m_currentFiles;  // 当前显示的文件列表

		//std::vector<std::shared_ptr<ResourceObject>> m_assetObjects;  //注册但未保存的资产对象

		float m_thumbnailSize;// 缩略图大小
		glm::vec4 m_assetColor; // 资产框颜色

		std::string m_currentPath;

		bool m_showAlreadySavedPopup;   //是否保存弹窗标志

		OnPathSelectedEvent m_pathSelectedEvent;
	};
}

#endif // !__volucris_asset_list_widget_h__
