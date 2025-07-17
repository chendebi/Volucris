#ifndef __volucris_mesh_loader_h__
#define __volucris_mesh_loader_h__

#include <string>
#include <memory>
#include <vector>
#include <Engine/Core/TypesHelp.h>

class aiScene;
class aiNode;

namespace volucris
{
	class StaticMesh;

	struct MeshResource
	{
		struct Slot
		{
			uint32 index;
			uint32 assimpMatIndex;
		};

		std::string name;
		std::shared_ptr<StaticMesh> mesh = nullptr;
		std::vector<Slot> slots;
	};

	struct MaterialResource
	{

	};

	struct TextureResource
	{

	};

	class MeshLoader
	{
	public:
		MeshLoader(const std::string& filepath);

		bool load();

		const std::vector<MeshResource>& getMeshes() const { return m_meshes; }

	private:
		MeshResource loadMeshFromNode(const aiScene* scene, aiNode* node);

	private:
		class AssimpHelper;
		std::string m_filepath;
		std::vector<MeshResource> m_meshes;
	};
}

#endif // !__volucris_mesh_loader_h__
