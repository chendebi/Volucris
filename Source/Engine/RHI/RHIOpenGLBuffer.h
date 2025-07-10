#ifndef __volucris_rhi_opengl_buffer_h__
#define __volucris_rhi_opengl_buffer_h__

#include <RHI/RHIOpenGL.h>
#include <Core/TypesHelp.h>

namespace volucris
{
	struct RHIOpenGLBuffer
	{
		GLenum target;
		GLenum usage;
		uint64 size;
	};
}

#endif // !__volucris_rhi_opengl_buffer_h__
