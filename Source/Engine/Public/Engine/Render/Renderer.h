#ifndef __volucris_renderer_h__
#define __volucris_renderer_h__

#include <Engine/Core/Runable.h>

namespace volucris
{
	class Window;
	class RHIRenderTarget;

	class Renderer : public Runable
	{
	public:
		static Renderer& getInstance()
		{
			static Renderer inst;
			return inst;
		}

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
		std::shared_ptr<RHIRenderTarget> m_renderTarget;
	};
}

#endif // !__volucris_renderer_h__
