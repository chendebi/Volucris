#ifndef __volucris_renderer_h__
#define __volucris_renderer_h__

#include <functional>
#include <memory>
#include "Engine/Core/circle_queue.h"
#include "Engine/Core/size.h"

namespace volucris
{
	using RenderCommand = std::function<void()>;

	class Context;
	class Scene;
	class ViewportProxy;
	class SceneProxy;
	class PrimitiveProxy;
	class PrimitiveComponent;

	class Renderer
	{
	public:
		Renderer();

		virtual ~Renderer();

		void render();

		void setContext(const std::shared_ptr<Context>& context);

		Context* getContext() const { return m_context.get(); }

		void setWindowFrameSize(int width, int height);

		void addScene(const std::shared_ptr<SceneProxy>& scene);

		void removeScene(Scene* scene);

		Size getFrameSize() const { return m_windowFrameSize; }

		PrimitiveProxy* createPrimitiveProxy(PrimitiveComponent* primitive);

		void pushCommand(RenderCommand command);

		void release();

		void clearCommands();

	private:
		void renderui();

	private:
		std::shared_ptr<Context> m_context;
		CircleQueue<RenderCommand> m_commands;
		std::vector<std::shared_ptr<ViewportProxy>> m_viewports;
		std::vector<std::shared_ptr<SceneProxy>> m_scenes;
		Size m_windowFrameSize;
	};
}

#endif // !__volucris_renderer_h__
