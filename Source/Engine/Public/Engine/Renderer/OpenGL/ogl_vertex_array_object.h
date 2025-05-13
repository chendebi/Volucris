#ifndef __volucris_ogl_vertex_array_object_h__
#define __volucris_ogl_vertex_array_object_h__

#include <vector>
#include <memory>
#include "Engine/Core/types_help.h"
#include <glad/glad.h>

namespace volucris
{
	class OGLBufferObject;
	class Context;

	struct BufferDescription
	{
		GLenum type;
		int size;
		bool normalized;
		int stride;
		int offset;
	};

	class OGLVertexArrayObject
	{
	public:
		OGLVertexArrayObject();

		~OGLVertexArrayObject();

		bool create();

		void release();

		void setDescriptions(const std::vector<BufferDescription>& descriptions)
		{
			m_descriptions = descriptions;
		}

		void setVertexBufferObject(const std::shared_ptr<OGLBufferObject>& vbo)
		{
			m_vbo = vbo;
		}

		bool initialize(Context* ctx);

		bool valid() const { return m_valid; }

		uint32 getID() const { return m_id; }

	private:
		uint32 m_id;
		uint8 m_valid;
		std::shared_ptr<OGLBufferObject> m_vbo;
		std::vector<BufferDescription> m_descriptions;
	};
}

#endif // !__volucris_ogl_vertex_array_object_h__
