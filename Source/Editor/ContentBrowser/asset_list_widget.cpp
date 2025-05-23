#include "asset_list_widget.h"
#include <imgui/imgui.h>

namespace volucris
{
	AssetListWidget::AssetListWidget()
	{
	}

	void AssetListWidget::build()
	{
		ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
		ImGui::Text("Right Content");
		ImGui::EndChild();
	}
}
