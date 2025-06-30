#ifndef __volucris_rhi_texture_h__
#define __volucris_rhi_texture_h__

#include <Engine/RHI/RHIResource.h>
#include <Engine/Core/Size.h>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	class RHICommandList;

	struct RHITextureDesc
	{
		enum ETextureClass
		{
			Texture,
			Texture2D
		};

		ETextureClass texClass = ETextureClass::Texture2D;
		Size size = { 0, 0 };
		Texture::EPixelFormat pixelFormat = Texture::EPixelFormat::Invalid;
		Texture::ESourceFormat sourceFormat = Texture::ESourceFormat::Invalid;


		static RHITextureDesc create2D(int width, int height, Texture::EPixelFormat format)
		{
			RHITextureDesc desc;
			desc.texClass = Texture2D;
			desc.size = { width, height };
			desc.pixelFormat = format;
			return desc;
		}
	};

	class RHITexture : public RHIResource
	{
	public:
		RHITexture();

	protected:
		uint32 create(RHIState* state) override;
	};

	class RHITexture2D : public RHITexture
	{
	public:
		RHITexture2D();

		RHITexture2D(const RHITextureDesc& desc);

		~RHITexture2D() override;

		bool init(RHICommandList* command) override;

		Texture::EPixelFormat getPixelFormat() const { return m_pixelFormat; }

		Size getSize() const { return m_size; }

	protected:
		void bind(RHIState* state) override;

		void destroy(RHIState* state) override;

	private:
		Size m_size;
		Texture::EPixelFormat m_pixelFormat;
	};

	std::shared_ptr<RHITexture> RHICreateTexture(const RHITextureDesc& desc);
}

#endif // !__volucris_rhi_texture_h__
