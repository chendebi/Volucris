#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_check.h"
#include "Renderer/context.h"

namespace volucris
{
	OGLBufferObject::OGLBufferObject(GLenum target, GLenum usage)
		: m_target(target)
		, m_usage(usage)
		, m_id(0)
		, m_data(nullptr)
		, m_size(0)
		, m_valid(false)
	{
		
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
	}

	void OGLBufferObject::setData(void* data, size_t size)
	{
		m_data = data;
		m_size = size;
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

		if (m_size == 0)
		{
			V_LOG_WARN(Engine, "initialize gl buffer failed. because not buffer size is 0");
			return false;
		}

		if (m_target == GL_ARRAY_BUFFER)
		{
			ctx->bindVertexBufferObject(this);
		}
		else if (m_target == GL_ELEMENT_ARRAY_BUFFER)
		{
			ctx->bindElementBufferObjct(this);
		}
		else
		{
			check(false);
		}
		glBufferData(m_target, m_size, m_data, m_usage);
		GL_CHECK();
		m_valid = true;
		return m_valid;
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

	OGLUniformBufferObject::OGLUniformBufferObject(size_t size)
		: OGLBufferObject(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW)
		, m_dirtyBlock()
	{
		setData(nullptr, size);
	}

	OGLUniformBufferObject::BlockID OGLUniformBufferObject::addBlock(void* data, size_t size)
	{
		/*auto offset = m_data.size();
		m_data.resize(offset + size);
		memcpy(m_data.data() + offset, data, size);*/
		//return { offset, size };
	}

	void OGLUniformBufferObject::updateBlock(BlockID id, void* data)
	{
		if (id.offset < m_dirtyBlock.start)
		{
			m_dirtyBlock.start = id.offset;
		}

		if (m_dirtyBlock.end <= id.offset)
		{
			m_dirtyBlock.end = id.offset + id.size;
		}
	}
}