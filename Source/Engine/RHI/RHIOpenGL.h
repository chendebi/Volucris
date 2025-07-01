#ifndef __volucris_rhi_opengl_h__
#define __volucris_rhi_opengl_h__

#include <glad/glad.h>
#include <Core/Assert.h>
#include <Core/Volucris.h>
#include <Core/TextureDefines.h>

#define GL_CHECK() \
	{\
		auto err = glGetError(); \
		while (err != GL_NO_ERROR) \
		{ \
			v_checkf(false, Engine, "gl error at {} : {}, code: {:X}", __FILE__, __LINE__, err); \
			err = glGetError(); \
		} \
	}

#define GL_CALL(exp) \
	exp; \
	GL_CHECK()

namespace volucris
{
	static GLenum getGLFormat(Texture::EPixelFormat format)
	{
		switch (format)
		{
		case volucris::Texture::EPixelFormat::R8G8B8:
			return GL_RGB8;
		case volucris::Texture::EPixelFormat::R8G8B8A8:
			return GL_RGBA8;
		default:
			break;
		}
		v_check(false)
		return GL_NONE;
	}
}

#endif // !__volucris_rhi_opengl_h__
