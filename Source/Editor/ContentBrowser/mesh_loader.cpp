#include "mesh_loader.h"
#include <Engine/Resource/static_mesh.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <EditorEntry/editor_core.h>

namespace volucris
{
	std::shared_ptr<StaticMesh> MeshLoader::load(const std::string& filePath)
	{
		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
			aiPrimitiveType_LINE | aiPrimitiveType_POINT);
		importer.SetPropertyBool(AI_CONFIG_IMPORT_COLLADA_IGNORE_UP_DIRECTION, true);
		importer.SetPropertyBool(AI_CONFIG_PP_PTV_KEEP_HIERARCHY, true);

		aiScene const* scene = importer.ReadFile(filePath,
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_GlobalScale |
			aiProcess_GenUVCoords |
			aiProcess_Triangulate
			);

		if (!scene)
		{
			V_LOG_WARN(Editor, "load mesh from file: {} failed.", filePath);
			return nullptr;
		}

		auto meshCount = scene->mNumMeshes;

		return nullptr;
	}
}
