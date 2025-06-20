#ifndef __volucris_texture2d_resource_h__
#define __volucris_texture2d_resource_h__

#include <vector>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Render/Resource.h>


namespace volucris
{
	class TextureResouece : public Resource
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
		TextureResouece() : Resource(), m_filter(EFilter::Near), m_wrapMode(EWrapMode::Repeat)
		{
			markDirty();
		}

		void setFilter(EFilter filter) { m_filter = filter; markDirty(); }

		void setWrapMode(EWrapMode mode) { m_wrapMode = mode; markDirty(); }

	private:
		EFilter m_filter;
		EWrapMode m_wrapMode;
	};

	class Texture2DResource : public TextureResouece
	{
	public:
		Texture2DResource();

		~Texture2DResource() override;

		void setTextureData(std::vector<uint8> data)
		{
			m_data = std::move(data);
			markDirty();
		}

	private:
		uint32 m_resourceID;
		std::vector<uint8> m_data;
	};
}

#endif // !__volucris_texture2d_resource_h__
