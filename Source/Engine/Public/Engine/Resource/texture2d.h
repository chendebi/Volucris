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

		Texture2D(TextureData data);

		~Texture2D();

		void update();

		std::shared_ptr<Texture2DProxy> getProxy();

		TextureData getTextureData() const { return m_data; }

		bool serialize(Serializer& serializer) const override { return false; }

		void deserialize(Serializer& serializer) {}

	private:
		uint8 m_dirty;
		TextureData m_data;
		std::weak_ptr<Texture2DProxy> m_proxy;
	};
}

#endif // !__volucris_texture_2d_h__
