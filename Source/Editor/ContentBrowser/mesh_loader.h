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

		bool load(const std::string& filePath);

		const std::vector<std::shared_ptr<ResourceObject>>& getLoadedResouces() const { return m_resources; }

		const std::vector<std::shared_ptr<StaticMesh>>& getLoadedStaticMeshes() const { return m_loadedMeshes; }

	protected:
		template<typename T>
		std::shared_ptr<T> createResource()
		{
			auto resource = std::make_shared<T>();
			m_resources.push_back(resource);
			return resource;
		}

	private:
		class AssimpHelper;
		std::vector<std::shared_ptr<ResourceObject>> m_resources;
		std::vector<std::shared_ptr<StaticMesh>> m_loadedMeshes;
	};
}

#endif // !__volucris_mesh_loader_h__
