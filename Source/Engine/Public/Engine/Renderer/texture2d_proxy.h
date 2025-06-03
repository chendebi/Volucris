#ifndef __volucris_texture2d_proxy_h__
#define __volucris_texture2d_proxy_h__

#include <memory>
#include <Core/size.h>
#include <Core/texture.h>
#include <Engine/Renderer/OpenGL/ogl_texture2d_object.h>

namespace volucris
{
	class Texture2D;

	class Texture2DProxy
	{
	public:
		Texture2DProxy();

		void initialize(Texture2D* texture);

		void setTextureData(TextureData data);

	private:
		std::unique_ptr<Texture2DObject> m_object;
	};
}

#endif // !__volucris_texture2d_proxy_h__
