#ifndef __volucris_rhi_commmand_list_h__
#define __volucris_rhi_commmand_list_h__

#include <memory>
#include <string>
#include <Engine/Core/Core.h>
#include <Engine/RHI/RHIState.h>

namespace volucris
{
	class Window;
	class RHIResource;

	struct RenderScope
	{
		RenderScope(const std::string& name);

		~RenderScope();
	};

	class RHICommandList
	{
	public:
		RHICommandList();

		~RHICommandList();

		bool initialize(Window* widnow, bool sync = false);

		void destroy();

		void makeCurrent();

		void swapBuffers();

		void clear(const RHIClearState& state);

		void executeCommand(const std::string& name);

		void setTexture(RHITexture* texture);

		void setRenderTarget(RHIRenderTarget* renderTarget);

		void bindResource(RHIResource* resource);

		void deleteResource(RHIResource* resource);

		void setViewport(int x, int y, int w, int h);

	private:
		Window* m_window;
		RHIState m_state;

		struct Impl;
		Impl* m_impl;
	};
}

#define RENDER_SCOPE(name) volucris::RenderScope V_UNIQUE_NAME(__renderScope_) = volucris::RenderScope(#name);

#endif // !__volucris_rhi_commmand_list_h__
