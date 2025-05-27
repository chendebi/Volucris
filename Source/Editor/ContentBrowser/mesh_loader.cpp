#include "mesh_loader.h"
#include <Engine/Resource/static_mesh.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <EditorEntry/editor_core.h>
#include <assimp/DefaultLogger.hpp>
#include <Engine/Resource/mesh_resource.h>
#include <Engine/Resource/mesh_resource_data.h>

namespace volucris
{
	inline glm::vec3 assimpVec3ToGlmVec3(const aiVector3D& pos)
	{
		return { pos.x, pos.y, pos.z };
	}

	static std::shared_ptr<MeshResource> buildMeshResource(aiMesh* mesh)
	{
		auto data = std::make_shared<MeshResourceData>();
		std::vector<glm::vec3> vertices;
		vertices.reserve(mesh->mNumVertices);
		glm::vec3 minPos, maxPos;
		minPos = maxPos = assimpVec3ToGlmVec3(mesh->mVertices[0]);
		for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
		{
			auto pos = assimpVec3ToGlmVec3(mesh->mVertices[idx]);
			minPos.x = minPos.x < pos.x ? minPos.x : pos.x;
			minPos.y = minPos.y < pos.y ? minPos.y : pos.y;
			minPos.z = minPos.z < pos.z ? minPos.z : pos.z;

			maxPos.x = maxPos.x > pos.x ? maxPos.x : pos.x;
			maxPos.y = maxPos.y > pos.y ? maxPos.y : pos.y;
			maxPos.z = maxPos.z > pos.z ? maxPos.z : pos.z;
			vertices.push_back(pos);
		}

		auto center = (minPos + maxPos) * glm::vec3(0.5,0.5,0.5);
		for (auto& vertex : vertices)
		{
			vertex = vertex - center;
		}

		data->setVertices(std::move(vertices));

		if (mesh->mNormals)
		{
			std::vector<glm::vec3> normals;
			normals.reserve(mesh->mNumVertices);
			for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
			{
				auto normal = glm::normalize(assimpVec3ToGlmVec3(mesh->mNormals[idx]));
				normals.push_back(normal);
			}
			data->setNormals(std::move(normals));
		}

		// 暂只支持两套UV
		for (auto i = 0; i < 2; ++i)
		{
			if (!mesh->mTextureCoords[i])
			{
				continue;
			}

			std::vector<glm::vec3> uvs;
			uvs.reserve(mesh->mNumVertices);
			for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
			{
				auto uv = assimpVec3ToGlmVec3(mesh->mTextureCoords[i][idx]);
				uvs.push_back(uv);
			}
			data->setUV(i, std::move(uvs));
		}

		return std::make_shared<MeshResource>(data);
	}

	MeshLoader::MeshLoader()
		: m_resources()
		, m_loadedMeshes()
	{
	}

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

		V_LOG_DEBUG(Editor, "load mesh from file: {}, mesh count: {}", filePath, meshCount);

		std::vector<std::shared_ptr<MeshResource>> resources;
		resources.reserve(scene->mNumMeshes);
		for (auto i = 0; i < scene->mNumMeshes; ++i)
		{
			auto resource = buildMeshResource(scene->mMeshes[i]);
			resources.push_back(resource);
		}

		// TODO: 材质解析

		
		return {};
	}
}
