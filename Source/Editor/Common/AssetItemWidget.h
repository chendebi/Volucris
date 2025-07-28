#ifndef __volucris_asset_item_widget_h__
#define __volucris_asset_item_widget_h__

#include <Engine/Application/Widget.h>
#include <Engine/Game/SoftObject.h>
#include <Engine/Application/Event.h>

namespace volucris
{
	DECLARE_EVENT_DELEGATE(PackageEvent, void, AssetData)

	class AssetItemWidget : public Widget
	{
	public:
		PackageEvent SelectedChanged;

	public:
		AssetItemWidget(const std::string& name);

		void setClassName(const std::string& className);

		void onBuild(bool) override;
	
		const AssetData& getAssetData() const;

	private:
		std::string m_name;
		std::string m_className;
		AssetData m_assetData;
		int m_height;
	};
}

#endif // !__volucris_asset_item_widget_h__
