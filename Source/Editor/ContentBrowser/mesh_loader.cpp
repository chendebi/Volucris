#include "mesh_loader.h"
#include <Engine/Resource/static_mesh.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <EditorEntry/editor_core.h>
#include <assimp/DefaultLogger.hpp>

namespace volucris
{
	std::vector<std::shared_ptr<StaticMesh>> MeshLoader::load(const std::string& filePath)
	{
		Assimp::DefaultLogger::create("", Assimp::Logger::NORMAL);
		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
			aiPrimitiveType_LINE | aiPrimitiveType_POINT);
		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
			aiComponent_LIGHTS | aiComponent_CAMERAS);
		importer.SetPropertyBool(AI_CONFIG_IMPORT_COLLADA_IGNORE_UP_DIRECTION, true);
		importer.SetPropertyBool(AI_CONFIG_PP_PTV_KEEP_HIERARCHY, true);
		aiScene const* scene = importer.ReadFile(filePath,
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_GlobalScale |
			aiProcess_GenUVCoords |
			aiProcess_Triangulate |
			aiProcess_RemoveComponent
			);

		if (!scene)
		{
			V_LOG_WARN(Editor, "load mesh from file: {} failed.", filePath);
			return {};
		}

		auto meshCount = scene->mNumMeshes;

		return {};
	}
}
