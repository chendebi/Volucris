#ifndef __volucris_viewport_widget_h__
#define __volucris_viewport_widget_h__

#include <Engine/Application/Widget.h>
#include <Engine/Core/TextureDefines.h>

namespace volucris
{
	class View;
	class Window;
	class RHITexture2D;
	class RHICommandList;
	class RHIWritePixelBuffer;

	class ViewportWidget : public Widget
	{
	public:
		ViewportWidget();

		void onTopWidgetChanged(Widget* old, Widget* current) override;

	protected:
		void onBuild() override;

		void viewSizeChanged(Size size);

		void onWindowAttachStateChanged(Window* window, bool attached);

	private:
		void recreateUploaders(RHICommandList* cmdList);

		void clearUploaders(RHICommandList* cmdList);

		void setViewData(Texture::TextureData data);

	private:
		View* m_view;
		Size m_size;
		Window* m_window;
		int m_current;
		std::shared_ptr<RHITexture2D> m_viewTexture;
		std::vector<std::shared_ptr<RHITexture2D>> m_textures;
		std::vector<std::shared_ptr<RHIWritePixelBuffer>> m_uploaders;
	};
}

#endif // !__volucris_viewport_widget_h__
