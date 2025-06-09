#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_check.h"
#include "Renderer/context.h"

namespace volucris
{
	OGLBufferObject::OGLBufferObject(GLenum target, GLenum usage)
		: m_target(target)
		, m_usage(usage)
		, m_id(0)
		, m_bufferSize(0)
		, m_valid(false)
		, m_shouldReallocate(true)
		, m_dirtyBlock()
		, m_data()
	{
		markShouldReallocateAtGPU();
	}

	OGLBufferObject::~OGLBufferObject()
	{
		release();
	}

	void OGLBufferObject::setUsage(GLenum usage)
	{
		if (usage == m_usage)
		{
			return;
		}
		m_usage = usage;
		markShouldReallocateAtGPU();
	}

	void OGLBufferObject::reserve(size_t size)
	{
		m_data.reserve(size);
		m_bufferSize = size;
		markShouldReallocateAtGPU();
	}

	void OGLBufferObject::setData(uint8* data, size_t size)
	{
		m_data.clear();
		addData(data, size);
	}

	OGLBufferObject::BlockID OGLBufferObject::addData(uint8* data, size_t size)
	{
		auto currSize = m_data.size();
		auto newSize = currSize + size;
		if (newSize != m_bufferSize)
		{
			reserve(newSize);
			m_data.resize(newSize);
		}
		memcpy(m_data.data() + currSize, data, size);
		return { currSize, size };
	}

	void OGLBufferObject::setBlockData(const BlockID& id, uint8* data)
	{
		if (id.offset + id.size > m_bufferSize)
		{
			V_LOG_WARN(Engine, "block id of buffer is invalid");
			return;
		}
		memcpy(m_data.data() + id.offset, data, id.size);
		dirtyBlock(id);
	}

	void OGLBufferObject::dirtyBlock(const BlockID& id)
	{
		if (id.offset < m_dirtyBlock.offset)
		{
			m_dirtyBlock.offset = id.offset;
		}

		auto dirtySize = id.offset + id.size - m_dirtyBlock.offset;
		if (dirtySize > m_dirtyBlock.size)
		{
			m_dirtyBlock.size = dirtySize;
		}
		m_valid = false;
	}

	bool OGLBufferObject::create()
	{
		if (m_id > 0)
		{
			return true;
		}

		glGenBuffers(1, &m_id);
		return true;
	}

	void OGLBufferObject::release()
	{
		if (m_id > 0)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	bool OGLBufferObject::initialize(Context* ctx)
	{
		if (m_valid)
		{
			return true;
		}

		if (m_id == 0)
		{
			V_LOG_WARN(Engine, "initialize gl buffer failed. because not created");
			return false;
		}

		if (m_bufferSize == 0)
		{
			V_LOG_WARN(Engine, "initialize gl buffer failed. because buffer size is 0");
			return false;
		}

		ctx->bindBuffer(this);

		if (m_shouldReallocate)
		{
			glBufferData(m_target, m_bufferSize, m_data.data(), m_usage);
		}
		else
		{
			glBufferSubData(m_target, m_dirtyBlock.offset, m_dirtyBlock.size, m_data.data() + m_dirtyBlock.offset);
		}
		m_dirtyBlock.offset = 0;
		m_dirtyBlock.size = 0;
		GL_CHECK();
		m_valid = true;
		m_shouldReallocate = false;
		return m_valid;
	}

	void OGLBufferObject::markShouldReallocateAtGPU()
	{
		m_shouldReallocate = true;
		m_valid = false;
	}

	bool OGLBufferObject::createBuffers(const std::vector<OGLBufferObject*>& buffers)
	{
		if (buffers.empty())
		{
			return false;
		}
		GLenum target = buffers[0]->m_target;
		std::vector<uint32> ids(buffers.size());
		for (const auto& buffer : buffers)
		{
			if (buffer->valid())
			{
				V_LOG_WARN(Engine, "can not create gl buffers because of some buffer has been created.")
				return false;
			}
			ids.push_back(0);
		}

		glGenBuffers(ids.size(), ids.data());
		for (auto idx = 0; idx < ids.size(); ++idx)
		{
			check(ids[idx] > 0);
			buffers[idx]->m_id = ids[idx];
		}
		return true;
	}

	void OGLBufferObject::releaseBuffers(const std::vector<OGLBufferObject*>& buffers)
	{
		if (buffers.empty())
		{
			return;
		}
		GLenum target = buffers[0]->m_target;
		std::vector<uint32> ids(buffers.size());
		for (const auto& buffer : buffers)
		{
			if (!buffer->valid())
			{
				V_LOG_WARN(Engine, "can not create gl buffers because of some buffer not created.")
				return;
			}
			ids.push_back(buffer->m_id);
		}

		glDeleteBuffers(ids.size(), ids.data());
		for (auto idx = 0; idx < ids.size(); ++idx)
		{
			buffers[idx]->m_id = 0;
		}
	}
}