#ifndef __volucris_content_browser_widget_h__
#define __volucris_content_browser_widget_h__

#include <Engine/Application/widget.h>

namespace volucris
{
	class PathTreeWidget;
	class AssetListWidget;

	class ContentBrowserWidget : public Widget
	{
	public:
		ContentBrowserWidget();

		void build() override;

	private:
		bool is_dragging = false;
		std::shared_ptr<AssetListWidget> m_assetWidget;
		std::shared_ptr<PathTreeWidget> m_pathWidget;
	};
}

#endif // !__volucris_content_browser_widget_h__
