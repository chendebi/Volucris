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
#include <Engine/Resource/material.h>
#include <Engine/Core/assert.h>
#include "material_loader.h"
#include <Engine/Resource/resource_registry.h>

namespace volucris
{
	inline glm::vec3 assimpVec3ToGlmVec3(const aiVector3D& pos)
	{
		return { pos.x, pos.y, pos.z };
	}

	struct ResourceBuildData
	{
		std::shared_ptr<MeshResource> resource = nullptr;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials = {};
	};

	class MeshLoader::AssimpHelper
	{
	public:
		MeshLoader* loader;
		const aiScene* scene;
		std::vector<std::shared_ptr<Material>> materials;
		std::vector<std::shared_ptr<StaticMesh>> staticMeshes;

		AssimpHelper(MeshLoader* meshLoader)
			: loader(meshLoader)
			, scene(nullptr)
		{

		}

		std::shared_ptr<Material> buildMaterial(aiMaterial* material)
		{
			MaterialLoader loader;
			std::string vsf, fsf;
			ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader/default_mesh.vert", vsf);
			ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader/default_mesh.frag", fsf);
			auto resource = loader.load(vsf, fsf);
			if (resource)
			{
				auto mat = this->loader->createResource<Material>(resource);
				auto asset = mat->getAsset();
				asset.name = material->GetName().C_Str();
				asset.assetPath = fmt::format("{}/{}", asset.path, asset.name);
				mat->setAsset(asset);
				return mat;
			}
			return nullptr;
		}

		void setScene(const aiScene* assimpScene)
		{
			scene = assimpScene;
			materials.reserve(scene->mNumMaterials);
			for (auto idx = 0; idx < scene->mNumMaterials; ++idx)
			{
				materials.push_back(nullptr);
			}
		}

		ResourceBuildData buildMeshResource(const std::vector<aiMesh*>& meshes)
		{
			auto data = std::make_shared<MeshResourceData>();
			bool buildUV0 = false;
			bool buildUV1 = false;
			size_t vertexCount = 0;
			size_t indexCount = 0;
			for (const auto& mesh : meshes)
			{
				if (!mesh->mNormals)
				{
					V_LOG_ERROR(Editor, "mesh not find normals");
					return {};
				}

				vertexCount += mesh->mNumVertices;
				indexCount += mesh->mNumFaces * 3;

				if (mesh->mTextureCoords[0])
				{
					buildUV0 = true;
				}

				if (mesh->mTextureCoords[1])
				{
					buildUV1 = true;
				}
			}

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> uv0;
			std::vector<glm::vec3> uv1;
			std::vector<uint32> indices;

			vertices.reserve(vertexCount);
			normals.reserve(vertexCount);
			indices.reserve(indexCount);

			if (buildUV0)
			{
				uv0.reserve(vertexCount);
			}
			if (buildUV1)
			{
				uv1.reserve(vertexCount);
			}

			ResourceBuildData res;
			glm::vec3 minPos, maxPos;
			minPos = maxPos = assimpVec3ToGlmVec3(meshes[0]->mVertices[0]);

			std::vector<Section> sections;
			for (const auto& mesh : meshes)
			{
				auto indexOffset = vertices.size();

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

				for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
				{
					auto normal = glm::normalize(assimpVec3ToGlmVec3(mesh->mNormals[idx]));
					normals.push_back(normal);
				}

				if (buildUV0)
				{
					for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
					{
						glm::vec3 uv = { 0,0,0 };
						if (auto texcoords = mesh->mTextureCoords[0])
						{
							uv = assimpVec3ToGlmVec3(texcoords[idx]);
						}
						uv0.push_back(uv);
					}
				}

				if (buildUV1)
				{
					for (auto idx = 0; idx < mesh->mNumVertices; ++idx)
					{
						glm::vec3 uv = { 0,0,0 };
						if (auto texcoords = mesh->mTextureCoords[1])
						{
							uv = assimpVec3ToGlmVec3(texcoords[idx]);
						}
						uv1.push_back(uv);
					}
				}

				auto count = mesh->mNumFaces * 3;

				Section section;
				section.mode = DrawMode::TRIANGLES;
				section.offset = indices.size();
				section.count = count;

				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				if (materials[mesh->mMaterialIndex] == nullptr)
				{
					materials[mesh->mMaterialIndex] = buildMaterial(material);
				}
				section.slot = material->GetName().C_Str();
				res.materials.insert({ section.slot, materials[mesh->mMaterialIndex] });
				data->addSection(section);

				for (auto k = 0; k < mesh->mNumFaces; ++k)
				{
					auto face = mesh->mFaces[k];
					v_checkf(face.mNumIndices == 3, Editor, "mesh parse error, find not triangle face, face size: {}", face.mNumIndices);
					indices.push_back(indexOffset + face.mIndices[0]);
					indices.push_back(indexOffset + face.mIndices[1]);
					indices.push_back(indexOffset + face.mIndices[2]);
				}
			}

			auto center = (minPos + maxPos) * glm::vec3(0.5, 0.5, 0.5);
			for (auto& vertex : vertices)
			{
				vertex = vertex - center;
			}

			data->setVertices(std::move(vertices));
			data->setNormals(std::move(normals));
			data->setSectionData(indices);
			if (buildUV0)
			{
				data->setUV(0, std::move(uv0));
			}
			if (buildUV1)
			{
				data->setUV(1, std::move(uv1));
			}
			res.resource = std::make_shared<MeshResource>(data);
			return res;
		}

		void buildStaticMesh(aiNode* node)
		{
			if (node->mNumMeshes > 0)
			{
				std::vector<aiMesh*> meshes;
				meshes.reserve(node->mNumMeshes);
				for (auto idx = 0; idx < node->mNumMeshes; ++idx)
				{
					meshes.push_back(scene->mMeshes[node->mMeshes[idx]]);
				}
				auto res = buildMeshResource(meshes);
				auto staticMesh = loader->createResource<StaticMesh>(res.resource);
				auto asset = staticMesh->getAsset();
				asset.name = node->mName.C_Str();
				asset.assetPath = fmt::format("{}/{}", asset.path, asset.name);
				staticMesh->setAsset(asset);

				for (const auto& [slot, material] : res.materials)
				{
					staticMesh->setMaterial(slot, material);
				}

				staticMeshes.push_back(staticMesh);
			}

			for (auto idx = 0; idx < node->mNumChildren; ++idx)
			{
				buildStaticMesh(node->mChildren[idx]);
			}
		}
	};

	MeshLoader::MeshLoader(const std::string& path)
		: m_path(path)
		, m_resources()
		, m_loadedMeshes()
	{
	}

	bool MeshLoader::load(const std::string& filePath)
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
			aiProcess_RemoveComponent |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
			);

		if (!scene)
		{
			V_LOG_WARN(Editor, "load mesh from file: {} failed.", filePath);
			return false;
		}

		auto meshCount = scene->mNumMeshes;

		AssimpHelper helper = AssimpHelper(this);
		helper.setScene(scene);
		helper.buildStaticMesh(scene->mRootNode);
		
		m_loadedMeshes = std::move(helper.staticMeshes);
		m_loadedMaterials = std::move(helper.materials);
		return true;
	}
}
