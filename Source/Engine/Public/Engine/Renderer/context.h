#ifndef __volucris_context_h__
#define __volucris_context_h__

#include "Engine/Renderer/OpenGL/ogl_render_state.h"

namespace volucris
{
	class Context
	{
	public:
		Context();

		~Context();

		void makeCurrent();

		void swapBuffers();

		void bindVertexBufferObject(OGLBufferObject* vbo);

		void bindVertexArrayObject(OGLVertexArrayObject* vao);

		void clear(const OGLClearState& state);

	private:
		struct Impl;
		Impl* m_impl;
		OGLRenderState m_renderState;
		OGLClearState m_clearState;
	};
}

#endif // !__volucris_context_h__
