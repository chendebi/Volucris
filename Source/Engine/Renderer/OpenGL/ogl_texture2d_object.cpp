#include "Renderer/OpenGL/ogl_texture2d_object.h"
#include "Renderer/context.h"
#include <Renderer/OpenGL/ogl_check.h>

namespace volucris
{
	Texture2DObject::Texture2DObject()
		: FrameBufferComponent(ComponentType::TEXTURE)
		, m_type(GL_UNSIGNED_BYTE)
		, m_filter(GL_LINEAR)
		, m_dataFormat(GL_NONE)
		, m_data()
	{
		
	}

	Texture2DObject::~Texture2DObject()
	{
		if (isValid())
		{
			release();
		}
	}

	bool Texture2DObject::create()
	{
		if (m_id == 0)
		{
			glGenTextures(1, &m_id);
		}
		return m_id > 0;
	}

	void Texture2DObject::release()
	{
		if (m_id > 0)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	void Texture2DObject::setData(std::vector<uint8> data)
	{
		m_data = std::move(data);
		dirty();
	}

	bool Texture2DObject::initialize(Context* context)
	{
		if (m_valid)
		{
			return true;
		}

		if (m_id == 0 || !m_size.isValid())
		{
			return false;
		}
		context->bindTexture2D(this);
		auto sourceFormat = m_format;
		if (m_dataFormat != GL_NONE)
		{
			sourceFormat = m_dataFormat;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_size.width, m_size.height, 0, sourceFormat, m_type, m_data.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
		GL_CHECK();
		return true;
	}
}
