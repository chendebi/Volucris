#ifndef __volucris_texture2d_resource_h__
#define __volucris_texture2d_resource_h__

#include <vector>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Render/Resource.h>


namespace volucris
{
	class FTexture : public Resource
	{
	public:
		enum EFilter
		{
			Near,
			Linear
		};

		enum EWrapMode
		{
			Repeat,
			ClampToBorder,
			ClampToEdge
		};

	public:
		FTexture() : Resource(), m_filter(EFilter::Near), m_wrapMode(EWrapMode::Repeat)
		{
			markDirty();
		}

		void setFilter(EFilter filter) { m_filter = filter; markDirty(); }

		void setWrapMode(EWrapMode mode) { m_wrapMode = mode; markDirty(); }

	private:
		EFilter m_filter;
		EWrapMode m_wrapMode;
	};

	class FTexture2D : public FTexture
	{
	public:
		FTexture2D();

		~FTexture2D() override;

		void setTextureData(std::vector<uint8> data)
		{
			m_data = std::move(data);
			markDirty();
		}

	private:
		std::vector<uint8> m_data;
	};
}

#endif // !__volucris_texture2d_resource_h__
