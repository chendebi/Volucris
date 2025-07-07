#ifndef __volucris_viewport_widget_h__
#define __volucris_viewport_widget_h__

#include <Engine/Application/Widget.h>
#include <Engine/Core/TextureDefines.h>

namespace volucris
{
	class View;
	class Universe;
	class Window;
	class RHITexture2D;
	class RHICommandList;
	class RHIWritePixelBuffer;

	class ViewportWidget : public Widget
	{
	public:
		ViewportWidget();

		void setUniverse(const std::shared_ptr<Universe>& universe);

	protected:
		void onBuild() override;

		void viewSizeChanged(Size size);

		void onRendererBuild(RHICommandList* cmdList) override;

		void onRendererDestroy(RHICommandList* cmdList) override;

	private:
		void recreateUploaders(RHICommandList* cmdList);

		void clearUploaders(RHICommandList* cmdList);

		void setViewData(Texture::TextureData data);

		void createView();

		void releaseView();

	private:
		View* m_view;
		Size m_size;
		int m_current;
		std::shared_ptr<RHITexture2D> m_viewTexture;
		std::vector<std::shared_ptr<RHITexture2D>> m_textures;
		std::vector<std::shared_ptr<RHIWritePixelBuffer>> m_uploaders;

	private:
		std::shared_ptr<Universe> m_universe;
	};
}

#endif // !__volucris_viewport_widget_h__
