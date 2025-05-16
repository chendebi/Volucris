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
		struct BlockID
		{
			size_t offset = 0;
			size_t size = 0;

			bool valid() const
			{
				return size > 0;
			}
		};

	public:
		OGLBufferObject(GLenum target, GLenum usage=GL_STATIC_DRAW);

		virtual ~OGLBufferObject();

		void setUsage(GLenum usage);

		void reserve(size_t size);

		void setData(uint8* data, size_t size);

		BlockID addData(uint8* data, size_t size);

		void setBlockData(const BlockID& id, uint8* data);

		void dirtyBlock(const BlockID& id);

		bool valid() const { return m_valid; }

		bool create();

		void release();

		bool initialize(Context* content);
		
		uint32 getID() const { return m_id; }

		GLenum getTarget() const { return m_target; }

		void markShouldReallocateAtGPU();

		static bool createBuffers(const std::vector<OGLBufferObject*>& buffers);

		static void releaseBuffers(const std::vector<OGLBufferObject*>& buffers);

	private:
		GLenum m_target;
		GLenum m_usage;
		uint32 m_id; // id
		size_t m_bufferSize; // 记录buffer的大小
		uint8 m_valid; // 是否能够直接使用
		uint8 m_shouldReallocate; // 是否要重建buffer
		BlockID m_dirtyBlock;
		std::vector<uint8> m_data;
	};
}

#endif // !__volucris_ogl_buffer_object_h__
