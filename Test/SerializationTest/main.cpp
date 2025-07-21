#include <Engine/Core/GUID.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <Engine/Core/Logging.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <memory> // 添加智能指针支持
#include <boost/serialization/shared_ptr.hpp>
#include <random>
#include <boost/serialization/vector.hpp>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/Assert.h>
#include <Engine/Game/MeshElements.h>
#include <Engine/Game/StaticMesh.h>

VOLUCRIS_STATIC_LOG(Serialization, Trace)

using namespace volucris;


int main()
{
    StaticMesh mesh;
	mesh.addSubMesh(std::make_unique<LargeMeshElements>());
	std::ofstream fout("D:\\Projects\\Volucris\\Binaries\\a.bin", std::ios::binary | std::ios::trunc);
	boost::archive::binary_oarchive oa(fout);

	oa << &mesh;
}