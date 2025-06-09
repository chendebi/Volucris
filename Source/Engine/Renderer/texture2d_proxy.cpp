#include "Renderer/texture2d_proxy.h"
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Resource/texture2d.h>

namespace volucris
{
	inline GLint getGLFormat(Texture::Format format)
	{
		switch (format)
		{
		case volucris::Texture::RGB:
			return GL_RGB;
		case volucris::Texture::RGBA:
			return GL_RGBA;
		default:
			break;
		}
		return GL_NONE;
	}

	Texture2DProxy::Texture2DProxy()
		: m_object(std::make_unique<Texture2DObject>())
	{
	}

	void Texture2DProxy::initialize(Texture2D* texture)
	{
		setTextureData(std::move(texture->getTextureData()));
	}

	void Texture2DProxy::setTextureData(TextureData data)
	{
		m_object->setFormat(data.hdr ? GL_RGBA16F : GL_RGBA);
		m_object->setDataFormat(getGLFormat(data.format));
		m_object->setType(data.hdr ? GL_FLOAT : GL_UNSIGNED_BYTE);
	}
}
