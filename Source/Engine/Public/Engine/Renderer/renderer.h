#ifndef __volucris_renderer_h__
#define __volucris_renderer_h__

#include <memory>

namespace volucris
{
	class Context;

	class Renderer
	{
	public:
		void render();

		void setContext(const std::shared_ptr<Context>& context);

		Context* getContext() const { return m_context.get(); }

	private:
		void renderui();

	private:
		std::shared_ptr<Context> m_context;
	};
}

#endif // !__volucris_renderer_h__
