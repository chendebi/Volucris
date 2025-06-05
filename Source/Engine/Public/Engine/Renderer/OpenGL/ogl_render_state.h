#ifndef __volucris_ogl_render_state_h__
#define __volucris_ogl_render_state_h__

#include <glm/glm.hpp>
#include <memory>
#include "Engine/Core/types_help.h"
#include "Engine/Renderer/OpenGL/ogl_buffer_object.h"
#include <Engine/Renderer/material_inner_data.h>

namespace volucris
{
	class OGLBufferObject;
	class OGLVertexArrayObject;
	class OGLProgramObject;
	class Texture2DObject;
	class RenderBufferObject;
	class FrameBufferObject;
	class MaterialParameterRenderData;

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
		glm::vec4 color = { 0,0,0,1 };
		float depth = 1.0;
	};

	struct UniformBlock
	{
		OGLBufferObject* ubo = nullptr;
		OGLBufferObject::BlockID block;

		bool valid() const
		{
			return ubo && block.valid();
		}
	};

	struct OGLProgramState
	{
		OGLProgramObject* program = nullptr;
		MaterialInnerParameters engineDatas = 0;
		MaterialParameterRenderData* renderData = nullptr;
	};

	struct OGLDrawState
	{
		OGLBufferObject* ebo = nullptr;
		OGLVertexArrayObject* vao = nullptr;
		OGLProgramState programState;
	};

	struct OGLRenderState
	{
		OGLBufferObject* vbo = nullptr;
		OGLBufferObject* ubo = nullptr;
		Texture2DObject* texture2d = nullptr;
		RenderBufferObject* rbo = nullptr;
		FrameBufferObject* fbo = nullptr;
		std::unordered_map<uint32, OGLBufferObject*> ubos;
		OGLDrawState drawState;
	};
}

#endif // !__volucris_ogl_render_state_h__
