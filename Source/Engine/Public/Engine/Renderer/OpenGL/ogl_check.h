#ifndef __volucris_ogl_check_h__
#define __volucris_ogl_check_h__

#include "Core/volucris.h"
#include "Core/assert.h"
#include <glad/glad.h>

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

#endif // !__volucris_ogl_check_h__
