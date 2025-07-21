#include <Game/MeshElements.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace volucris
{
	SmallMeshElements* MeshElements::asSmall()
	{
		return dynamic_cast<SmallMeshElements*>(this);
	}

	MediumMeshElements* MeshElements::asMedium()
	{
		return dynamic_cast<MediumMeshElements*>(this);
	}

	LargeMeshElements* MeshElements::asLarge()
	{
		return dynamic_cast<LargeMeshElements*>(this);
	}

	uint32& LargeMeshElements::get(size_t index)
	{
		if (m_elements.size() <= index)
		{
			m_elements.resize(index + 1);
		}
		return m_elements[index];
	}
}

BOOST_CLASS_EXPORT(volucris::MeshElements)
BOOST_CLASS_EXPORT_IMPLEMENT(volucris::SmallMeshElements)
BOOST_CLASS_EXPORT_IMPLEMENT(volucris::MediumMeshElements)
BOOST_CLASS_EXPORT_IMPLEMENT(volucris::LargeMeshElements)