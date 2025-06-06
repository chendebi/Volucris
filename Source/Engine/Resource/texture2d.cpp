#include "Resource/texture2d.h"
#include <Application/application.h>
#include <Renderer/renderer.h>
#include <Core/volucris.h>
#include <Renderer/texture2d_proxy.h>

namespace volucris
{
	inline size_t getFormatComponentCount(Texture::Format format)
	{
		switch (format)
		{
		case volucris::Texture::RGB:
			return 3;
		case volucris::Texture::RGBA:
			return 4;
		default:
			break;
		}
		return 0;
	}

	Texture2D::Texture2D()
		: ResourceObject(Asset::TEXTURE)
		, m_dirty(false)
		, m_data()
		, m_proxy()
	{
	}

	Texture2D::Texture2D(uint8* data, Size size, Texture::Format format)
		: ResourceObject(Asset::TEXTURE)
		, m_dirty(false)
		, m_data()
		, m_proxy()
	{
		setSize(size.width, size.height);
		setFormat(format);
	}

	Texture2D::~Texture2D()
	{

	}

	void Texture2D::setData(uint8* data, size_t size)
	{
		m_data.data.resize(size);
		memcpy(m_data.data.data(), data, size);
	}

	void Texture2D::update()
	{
		if (m_dirty && !m_proxy.expired())
		{
			auto disired = getFormatComponentCount(m_data.format) * (m_data.hdr ? sizeof(float) : sizeof(uint8)) * m_data.size.width * m_data.size.height;
			if (m_data.data.size() != disired || m_data.data.empty())
			{
				V_LOG_WARN(Engine, "try update a invalid texture2d");
				m_dirty = false;
				return;
			}

			if (auto proxy = m_proxy.lock())
			{
				gApp->getRenderer()->pushCommand([proxy, data = m_data]() {
					proxy->setTextureData(data);
					});
				m_dirty = false;
			}
		}
	}

	std::shared_ptr<Texture2DProxy> Texture2D::getProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<Texture2DProxy>();
			proxy->initialize(this);
			m_proxy = proxy;
		}
		return proxy;
	}
}
