#ifndef __volucris_texture2d_h__
#define __volucris_texture2d_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Core/Size.h>
#include <Engine/Core/TextureDefines.h>
#include <boost/serialization/vector.hpp>

namespace volucris
{
	class Texture2D : public GameObject
	{
	public:
		Texture2D();

		Texture2D(Texture::TextureData data);
		
		virtual ~Texture2D();

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			volatile int break_here = 0; // 断点行
			ar & boost::serialization::base_object<GameObject>(*this);
			ar & m_data;
		}
		
	private:
		Texture::TextureData m_data;
	};
} // namespace volucris

BOOST_CLASS_EXPORT_KEY(volucris::Texture2D)

#endif // !__volucris_texture2d_h__
