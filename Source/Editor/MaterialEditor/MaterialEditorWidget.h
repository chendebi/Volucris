#ifndef __volucris_material_editor_widget_h__
#define __volucris_material_editor_widget_h__

#include <Engine/Application/Widget.h>

namespace volucris
{
	class Universe;
	class ViewportWidget;

	class MaterialEditorWidget : public Widget
	{
	public:
		MaterialEditorWidget();

	protected:
		void onBuild() override;

		void onWindowFocusChanged(FocusEvent* event) override;

	private:
		std::shared_ptr<ViewportWidget> m_viewport;
		std::shared_ptr<Universe> m_universe;
	};
}

#endif // !__volucris_material_editor_widget_h__
