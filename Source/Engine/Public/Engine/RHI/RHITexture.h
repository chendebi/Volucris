#ifndef __volucris_rhi_texture_h__
#define __volucris_rhi_texture_h__

#include <Engine/RHI/RHIResource.h>
#include <Engine/Core/Size.h>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Core/Rect.h>

namespace volucris
{
	class RHICommandList;

	enum class TextureType
	{
		Texture2D
	};

	struct RHITextureDesc
	{
		TextureType texClass = TextureType::Texture2D;
		Size size = { 0, 0 };
		Texture::EPixelFormat pixelFormat = Texture::EPixelFormat::Invalid;
		Texture::ESourceFormat sourceFormat = Texture::ESourceFormat::Invalid;


		static RHITextureDesc create2D(int width, int height, Texture::EPixelFormat format)
		{
			RHITextureDesc desc;
			desc.texClass = TextureType::Texture2D;
			desc.size = { width, height };
			desc.pixelFormat = format;
			return desc;
		}
	};

	class RHITexture : public RHIResource
	{
	public:
		RHITexture();

		~RHITexture();

		void createGpuResource();

		TextureType getType() const { return m_type; }

		uint32 getId() const { return m_id; }

	private:
		TextureType m_type;
		uint32 m_id;
	};

	class RHITexture2D : public RHITexture
	{
	public:
		RHITexture2D();

		RHITexture2D(const RHITextureDesc& desc);

		~RHITexture2D() override;

		bool init();

		void update(const Rect& rect,  const std::vector<uint8>& data);

		Texture::EPixelFormat getPixelFormat() const { return m_pixelFormat; }

		Size getSize() const { return m_size; }

	private:
		Size m_size;
		Texture::EPixelFormat m_pixelFormat;
	};

	std::shared_ptr<RHITexture> RHICreateTexture(const RHITextureDesc& desc);
}

#endif // !__volucris_rhi_texture_h__
