#ifndef __volucris_mesh_loader_h__
#define __volucris_mesh_loader_h__

#include <memory>
#include <vector>
#include <string>

namespace volucris
{
	class StaticMesh;
	class ResourceObject;

	class MeshLoader
	{
	public:
		MeshLoader();

		std::vector<std::shared_ptr<StaticMesh>> load(const std::string& filePath);

	private:
		std::vector<std::shared_ptr<ResourceObject>> m_resources;
		std::vector<std::shared_ptr<StaticMesh>> m_loadedMeshes;
	};
}

#endif // !__volucris_mesh_loader_h__
