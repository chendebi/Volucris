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
		~RHICommandList();

		static RHICommandList& getInstance()
		{
			static RHICommandList inst;
			return inst;
		}

		bool initialize(std::unique_ptr<Window> widnow);

		void destroy();

		void clear(const RHIClearState& state);

		void executeCommand(const std::string& name);

		void setTexture(RHITexture* texture);

		void setRenderTarget(RHIRenderTarget* renderTarget);

		void bindResource(RHIResource* resource);

		void deleteResource(RHIResource* resource);

		void setViewport(int x, int y, int w, int h);

	private:
		std::unique_ptr<Window> m_window;
		RHIState m_state;

		struct Impl;
		Impl* m_impl;

	private:
		RHICommandList();

		RHICommandList(const RHICommandList&) = delete;
		RHICommandList(RHICommandList&&) = delete;
		RHICommandList& operator=(const RHICommandList&) = delete;
		RHICommandList& operator=(RHICommandList&&) = delete;
	};
}

#define RHICmdList volucris::RHICommandList::getInstance()

template<typename F>
void process_lambda(F&& lambda) {
	auto& cmdList = RHICmdList;
	F(&cmdList);
}

#define ENQUEUE_COMMMAND_LIST(name, function) {\
	RHICmdList.executeCommand(#name);\
	auto& cmdList = RHICmdList;\
	function(&cmdList);\
}

#define RENDER_SCOPE(name) volucris::RenderScope V_UNIQUE_NAME(__renderScope_) = volucris::RenderScope(#name);

#endif // !__volucris_rhi_commmand_list_h__
