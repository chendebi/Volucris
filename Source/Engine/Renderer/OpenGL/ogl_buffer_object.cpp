#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_check.h"
#include "Renderer/context.h"

namespace volucris
{
	OGLBufferObject::OGLBufferObject(GLenum target)
		: m_target(target)
		, m_usage(GL_STATIC_DRAW)
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
			return false;
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

		ctx->bindVertexBufferObject(this);
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
}