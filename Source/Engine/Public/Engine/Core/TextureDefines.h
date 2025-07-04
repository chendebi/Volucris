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
			ESourceFormat format = ESourceFormat::Invalid;
			Size size = {};
			std::vector<uint8> data = {};

			template <class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar& format;
				ar& size;
				ar& data;
			}
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

		static EPixelFormat getPixelFormat(ESourceFormat format)
		{
			switch (format)
			{
			case volucris::Texture::ESourceFormat::RGB:
				return EPixelFormat::R8G8B8;
			case volucris::Texture::ESourceFormat::RGBA:
				return EPixelFormat::R8G8B8A8;
			default:
				break;
			}
			return EPixelFormat::Invalid;
		}
	}
}

#endif // !__volucris_texture_defines_h__
