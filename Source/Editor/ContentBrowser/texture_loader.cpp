#include "texture_loader.h"
#include <Engine/Resource/texture2d.h>
#include <stb_image/stb_image.h>
#include <EditorEntry/editor_core.h>

namespace volucris
{
	std::shared_ptr<Texture2D> TextureLoader::load(const std::string& filepath, bool hdr)
	{
		if (hdr)
		{
			return loadHDRImage(filepath);
		}
		return loadNormalImage(filepath);
	}

	std::shared_ptr<Texture2D> TextureLoader::loadHDRImage(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1); // OpenGL纹理坐标原点在左下

		int width, height, channels;
		float* bytes = nullptr;
		bytes = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
		if (bytes == nullptr) {
			V_LOG_WARN(Editor, "load image from {} failed.", filepath);
			V_LOG_WARN(Editor, " {}", stbi_failure_reason());
			return nullptr;
		}

		Texture::Format format = Texture::UNKNOWN;
		if (channels == 3)
		{
			format = Texture::RGB;
		}
		else if (channels == 4)
		{
			format = Texture::RGBA;
		}

		if (format == Texture::UNKNOWN)
		{
			V_LOG_WARN(Editor, "load image from {} failed. unsupport channel: {}", filepath, channels);
			return nullptr;
		}

		if (width == 0 || height == 0)
		{
			V_LOG_WARN(Editor, "load image from {} failed. invalid size: {}, {}", filepath, width, height);
			return nullptr;
		}

		size_t size = channels * width * height * sizeof(float);
		TextureData data;
		data.format = format;
		data.hdr = true;
		data.size = { width, height };
		data.data.resize(size);
		memcpy(data.data.data(), bytes, size);
		auto texture = std::make_shared<Texture2D>(std::move(data));
		stbi_image_free(bytes);
		return texture;
	}

	std::shared_ptr<Texture2D> TextureLoader::loadNormalImage(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1); // OpenGL纹理坐标原点在左下

		int width, height, channels;
		uint8* bytes = nullptr;
		bytes = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		if (bytes == nullptr) {
			V_LOG_WARN(Editor, "load image from {} failed.", filepath);
			V_LOG_WARN(Editor, " {}", stbi_failure_reason());
			return nullptr;
		}

		Texture::Format format = Texture::UNKNOWN;
		if (channels == 3)
		{
			format = Texture::RGB;
		}
		else if (channels == 4)
		{
			format = Texture::RGBA;
		}

		if (format == Texture::UNKNOWN)
		{
			V_LOG_WARN(Editor, "load image from {} failed. unsupport channel: {}", filepath, channels);
			return nullptr;
		}

		if (width == 0 || height == 0)
		{
			V_LOG_WARN(Editor, "load image from {} failed. invalid size: {}, {}", filepath, width, height);
			return nullptr;
		}

		size_t size = channels * width * height;
		TextureData data;
		data.format = format;
		data.hdr = false;
		data.size = { width, height };
		data.data.resize(size);
		memcpy(data.data.data(), bytes, size);
		auto texture = std::make_shared<Texture2D>(std::move(data));
		stbi_image_free(bytes);
		return texture;
	}
}