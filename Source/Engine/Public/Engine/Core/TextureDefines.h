#ifndef __volucris_texture_defines_h__
#define __volucris_texture_defines_h__

#include <Engine/Core/TypesHelp.h>
#include <vector>
#include <Engine/Core/Size.h>

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

		struct TextureData
		{
			ESourceFormat format;
			Size size;
			std::vector<uint8> data;
		};

		static ESourceFormat getSourceFormat(EPixelFormat format)
		{
			switch (format)
			{
			case volucris::Texture::EPixelFormat::R8G8B8:
				return ESourceFormat::RGB;
			case volucris::Texture::EPixelFormat::R8G8B8A8:
				return ESourceFormat::RGBA;
			default:
				break;
			}
			return ESourceFormat::Invalid;
		}
	}
}

#endif // !__volucris_texture_defines_h__
