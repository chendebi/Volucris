#ifndef __volucris_mesh_loader_h__
#define __volucris_mesh_loader_h__

#include <memory>
#include <vector>
#include <string>

namespace volucris
{
	class Material;
	class StaticMesh;
	class ResourceObject;

	class MeshLoader
	{
	public:
		MeshLoader(const std::string& path);

		bool load(const std::string& filePath);

		const std::vector<std::shared_ptr<ResourceObject>>& getLoadedResouces() const { return m_resources; }

		const std::vector<std::shared_ptr<StaticMesh>>& getLoadedStaticMeshes() const { return m_loadedMeshes; }

	protected:
		template<typename T, typename ...Args>
		std::shared_ptr<T> createResource(Args&&... args)
		{
			auto resource = std::make_shared<T>(std::forward<Args>(args)...);
			auto asset = resource->getAsset();
			asset.path = m_path;
			resource->setAsset(asset);
			m_resources.push_back(resource);
			return resource;
		}

	private:
		class AssimpHelper;
		std::string m_path;
		std::vector<std::shared_ptr<ResourceObject>> m_resources;
		std::vector<std::shared_ptr<StaticMesh>> m_loadedMeshes;
		std::vector<std::shared_ptr<Material>> m_loadedMaterials;
	};
}

#endif // !__volucris_mesh_loader_h__
