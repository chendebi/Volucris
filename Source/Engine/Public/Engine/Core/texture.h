#ifndef __volucris_texture_h__
#define __volucris_texture_h__

#include <vector>
#include <Engine/Core/size.h>
#include <Engine/Core/types_help.h>

namespace volucris
{

	class Texture
	{
	public:
		enum Format
		{
			UNKNOWN,
			RGB,
			RGBA
		};
	};

	struct TextureData
	{
		uint8 hdr;
		Texture::Format format;
		Size size;
		std::vector<uint8> data;
	};
}

#endif // !__volucris_texture_h__
