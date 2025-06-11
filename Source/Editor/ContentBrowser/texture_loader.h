#ifndef __volucris_texture_loader_h__
#define __volucris_texture_loader_h__

#include <memory>
#include <string>

namespace volucris
{
	class Texture2D;

	class TextureLoader
	{
	public:
		std::shared_ptr<Texture2D> load(const std::string& filepath, bool hdr = false);

	private:
		std::shared_ptr<Texture2D> loadHDRImage(const std::string& filepath);

		std::shared_ptr<Texture2D> loadNormalImage(const std::string& filepath);
	};
}

#endif // !__volucris_texture_loader_h__
