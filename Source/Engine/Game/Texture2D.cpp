#include <Game/Texture2D.h>
// 必须包含的存档类型
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
namespace volucris
{
	Texture2D::Texture2D()
		: GameObject()
		, m_data()
	{
	}

	Texture2D::Texture2D(Texture::TextureData data)
		: GameObject()
		, m_data(std::move(data))
	{
	}

	Texture2D::~Texture2D()
	{
	}


} // namespace volucris

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::Texture2D)
