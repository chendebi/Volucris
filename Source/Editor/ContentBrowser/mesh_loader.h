#ifndef __volucris_mesh_loader_h__
#define __volucris_mesh_loader_h__

#include <memory>
#include <vector>
#include <string>

namespace volucris
{
	class StaticMesh;

	class MeshLoader
	{
	public:
		MeshLoader() = default;

		std::vector<std::shared_ptr<StaticMesh>> load(const std::string& filePath);
	};
}

#endif // !__volucris_mesh_loader_h__
