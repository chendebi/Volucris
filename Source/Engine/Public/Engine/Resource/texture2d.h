#ifndef __volucris_texture_2d_h__
#define __volucris_texture_2d_h__

#include <Engine/Core/texture.h>
#include <Engine/Resource/resource_object.h>
#include <Engine/Core/size.h>
#include <vector>

namespace volucris
{
	class Texture2DProxy;

	class Texture2D : public ResourceObject
	{
	public:
		Texture2D();

		Texture2D(uint8* data, Size size, Texture::Format format);

		~Texture2D();

		void setSize(int width, int height) { m_data.size = Size(width, height); m_dirty = true; }

		void setFormat(Texture::Format format) { m_data.format = format; m_dirty = true;}
		
		void setData(uint8* data, size_t size);

		void update();

		std::shared_ptr<Texture2DProxy> getProxy();

		TextureData getTextureData() const { return m_data; }

	private:
		uint8 m_dirty;
		TextureData m_data;
		std::weak_ptr<Texture2DProxy> m_proxy;
	};
}

#endif // !__volucris_texture_2d_h__
