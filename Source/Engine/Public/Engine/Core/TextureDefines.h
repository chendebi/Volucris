#ifndef __volucris_texture_defines_h__
#define __volucris_texture_defines_h__

namespace volucris
{
	namespace Texture
	{
		enum class EPixelFormat
		{
			Invalid,
			R8G8B8, 
			R8G8B8A8
		};

		enum class ESourceFormat
		{
			Invalid,
			RGB,
			RGBA
		};
	}
}

#endif // !__volucris_texture_defines_h__
