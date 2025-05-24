#ifndef __volucris_render_buffer_object_h__
#define __volucris_render_buffer_object_h__

#include <Engine/Renderer/OpenGL/ogl_framebuffer_component.h>

namespace volucris
{
	class Context;

	class RenderBufferObject : public FrameBufferComponent
	{
	public:
		RenderBufferObject();

		~RenderBufferObject();

		bool create() override;

		void release();

		bool initialize(Context* context) override;
	};
}

#endif // !__volucris_render_buffer_object_h__
