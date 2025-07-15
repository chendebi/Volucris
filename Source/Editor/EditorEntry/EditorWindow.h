#ifndef __volucris_editor_window_h__
#define __volucris_editor_window_h__

#include <Engine/Application/Window.h>

namespace volucris
{
	class ViewportWidget;
	class LogWidget;

	class MainWidget : public Widget
	{
	public:
		MainWidget();

	protected:
		void onBuild() override;

	private:
		void buildMenubar();

	private:
		std::shared_ptr<ViewportWidget> m_viewport;
		std::shared_ptr<LogWidget> m_logWidget;
	};

	class EditorWindow : public Window
	{
	public:
		EditorWindow();

		void onRendererBuild(RHICommandList* cmdList) override;

		void onRendererDestroy(RHICommandList* cmdList) override;

	private:
		std::shared_ptr<Widget> m_widget;
	};
}

#endif // !__volucris_editor_window_h__
