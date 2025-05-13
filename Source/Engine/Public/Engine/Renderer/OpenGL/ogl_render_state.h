#ifndef __volucris_ogl_render_state_h__
#define __volucris_ogl_render_state_h__

#include <glm/glm.hpp>

namespace volucris
{
	class OGLBufferObject;
	class OGLVertexArrayObject;

	struct OGLClearState
	{
		enum Buffer
		{
			COLOR_BUFFER = 0x01,
			DEPTH_BUFFER = 0x02,
			COLOR_AND_DEPTH_BUFFER = COLOR_BUFFER | DEPTH_BUFFER,
			ALL = COLOR_BUFFER | DEPTH_BUFFER
		};

		int clearBuffers = Buffer::ALL;
		glm::vec4 color = { 0,0,0,0 };
		float depth = 1.0;
	};

	struct OGLRenderState
	{
		OGLBufferObject* vbo = nullptr;
		OGLBufferObject* ebo = nullptr;
		OGLVertexArrayObject* vao = nullptr;
	};
}

#endif // !__volucris_ogl_render_state_h__
