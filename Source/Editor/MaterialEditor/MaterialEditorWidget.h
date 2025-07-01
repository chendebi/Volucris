#ifndef __volucris_material_editor_widget_h__
#define __volucris_material_editor_widget_h__

#include <Engine/Application/Widget.h>

namespace volucris
{
	class ViewportWidget;

	class MaterialEditorWidget : public Widget
	{
	public:
		MaterialEditorWidget();


	protected:
		void onBuild() override;

		void onTopWidgetChanged(Widget* old, Widget* current) override;

	private:
		std::shared_ptr<ViewportWidget> m_viewport;
	};
}

#endif // !__volucris_material_editor_widget_h__
