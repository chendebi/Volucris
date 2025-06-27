#ifndef __volucris_renderer_h__
#define __volucris_renderer_h__

#include <Engine/Core/Runable.h>

namespace volucris
{
	class Window;
	class Frame;
	class RHICommandList;

	class Renderer : public Runable
	{
	public:
		~Renderer();

		static Renderer& getInstance()
		{
			static Renderer inst;
			return inst;
		}

		RHICommandList* getRenderCommand() { return m_cmdList.get(); }

	protected:
		Renderer();
		void main();

	private:
		friend class Application;
		void run() override;

		bool initialize() override;
		   
		void destroy() override;

	protected:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Frame> m_frame;
		std::unique_ptr<RHICommandList> m_cmdList;
	};
}

#define RHICmdList volucris::Renderer::getInstance().getRenderCommand()

#endif // !__volucris_renderer_h__
