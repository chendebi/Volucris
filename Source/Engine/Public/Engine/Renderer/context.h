#ifndef __volucris_context_h__
#define __volucris_context_h__

#include "Engine/Renderer/OpenGL/ogl_render_state.h"
#include "Engine/Renderer/mesh_render_data.h"
#include "Engine/Core/rect.h"
#include "Engine/Renderer/primitive_proxy.h"

namespace volucris
{
	class MaterialProxy;

	class Context
	{
	public:
		Context();

		~Context();

		void makeCurrent();

		void swapBuffers();

		void bindVertexBufferObject(OGLBufferObject* vbo);

		void bindElementBufferObjct(OGLBufferObject* ebo);

		void bindVertexArrayObject(OGLVertexArrayObject* vao);

		void setViewport(int x, int y, int w, int h);

		void setViewport(const Rect& rect);

		void clear(const OGLClearState& state);

		void draw(const OGLDrawState& state, const SectionRenderData& section);

		void draw2(const MaterialProxy* material, const SectionDrawData& section);

	protected:
		bool prepareDrawState(const OGLDrawState& state);

	private:
		struct Impl;
		Impl* m_impl;
		OGLRenderState m_renderState;
		OGLClearState m_clearState;
		Rect m_viewport;
	};
}

#endif // !__volucris_context_h__
