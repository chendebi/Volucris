#ifndef __volucris_ogl_buffer_object_h__
#define __volucris_ogl_buffer_object_h__

#include <vector>
#include <glad/glad.h>
#include "Engine/Core/types_help.h"

namespace volucris
{
	class Context;
	class OGLBufferObject
	{
	public:
		OGLBufferObject(GLenum target);

		virtual ~OGLBufferObject();

		void setUsage(GLenum usage);

		void setData(void* data, size_t size);

		bool valid() const { return m_valid; }

		bool create();

		void release();

		bool initialize(Context* content);
		
		uint32 getID() const { return m_id; }

		GLenum getTarget() const { return m_target; }

		static bool createBuffers(const std::vector<OGLBufferObject*>& buffers);

		static void releaseBuffers(const std::vector<OGLBufferObject*>& buffers);

	private:
		GLenum m_target;
		GLenum m_usage;
		uint32 m_id;
		void* m_data;
		size_t m_size;
		uint8 m_valid;
	};
}

#endif // !__volucris_ogl_buffer_object_h__
