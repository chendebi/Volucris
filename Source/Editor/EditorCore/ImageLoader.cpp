#include <EditorCore/ImageLoader.h>
#include <stb_image/stb_image.h>
#include <EditorCore/Editor.h>

namespace volucris
{
	bool ImageLoader::load()
	{
		return loadNormalImage(m_filePath);
	}

	bool ImageLoader::loadNormalImage(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1); // OpenGL纹理坐标原点在左下

		int width, height, channels;
		uint8* bytes = nullptr;
		bytes = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		if (bytes == nullptr) {
			V_LOG_WARN(Editor, "load image from {} failed.", filepath);
			V_LOG_WARN(Editor, " {}", stbi_failure_reason());
			return false;
		}

		Texture::ESourceFormat format = Texture::ESourceFormat::Invalid;
		if (channels == 3)
		{
			format = Texture::ESourceFormat::RGB;
		}
		else if (channels == 4)
		{
			format = Texture::ESourceFormat::RGBA;
		}

		if (format == Texture::ESourceFormat::Invalid)
		{
			V_LOG_WARN(Editor, "load image from {} failed. unsupport channel: {}", filepath, channels);
			return false;
		}

		if (width == 0 || height == 0)
		{
			V_LOG_WARN(Editor, "load image from {} failed. invalid size: {}, {}", filepath, width, height);
			return false;
		}

		size_t size = channels * width * height;
		m_textureData.format = format;
		//data.hdr = false;
		m_textureData.size = { width, height };
		m_textureData.data.resize(size);
		memcpy(m_textureData.data.data(), bytes, size);
		stbi_image_free(bytes);
		return true;
	}
}