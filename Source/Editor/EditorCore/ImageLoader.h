#ifndef __volucris_image_loader_h__
#define __volucris_image_loader_h__

#include <string>
#include <Engine/Core/TextureDefines.h>

namespace volucris
{
	class ImageLoader
	{
	public:
		ImageLoader(const std::string& filepath)
			: m_filePath(filepath)
		{
		}

		bool load();

		const Texture::TextureData& getTextureData() const { return m_textureData; }

	private:
		bool loadNormalImage(const std::string& filepath);

	private:
		std::string m_filePath;
		Texture::TextureData m_textureData;
	};
}

#endif // !__volucris_image_loader_h__
