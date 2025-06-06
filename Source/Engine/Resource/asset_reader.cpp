#include <Resource/asset_reader.h>
#include <Core/volucris.h>
#include <Resource/material.h>
#include <Resource/texture2d.h>
#include <Resource/static_mesh.h>
#include <Resource/asset_header.h>

namespace volucris
{
	static bool VerifyHeaderInfo(std::ifstream& fin)
	{
		AssetHeader header;
		fin.read((char*)&header, sizeof(header));
		if (fin.gcount() < 4)
		{
			return false;
		}

		if (header != AssetHeader::VerifyHeader)
		{
			V_LOG_WARN(Engine, "load asset failed, , file corrupted. version: {}, current: {}", header.version, AssetHeader::VerifyHeader.version);
			return false;
		}
		return true;
	}

	static bool LoadAssetMeta(std::ifstream& fin, Asset& asset)
	{
		uint32 metaSize;
		fin.read((char*)&metaSize, sizeof(uint32));
		if (fin.gcount() < sizeof(uint32))
		{
			return false;
		}

		std::vector<uint8> metaData;
		metaData.resize(metaSize);
		fin.read((char*)metaData.data(), metaSize);
		if (fin.gcount() < metaSize)
		{
			return false;
		}

		Serializer serializer;
		serializer.setData(std::move(metaData));

		return serializer.deserialize(asset);
	}

	static bool loadContentData(std::ifstream& fin, std::vector<uint8>& data)
	{
		// 读取资源内容
		uint32 contentSize = 0;
		fin.read((char*)&contentSize, sizeof(uint32));
		if (fin.gcount() < sizeof(uint32))
		{
			return false;
		}

		std::vector<uint8> contentData(contentSize);
		fin.read((char*)contentData.data(), contentSize);
		if (fin.gcount() < contentSize)
		{
			return false;
		}
		data = std::move(contentData);
		return true;
	}

	std::shared_ptr<ResourceObject> AssetReader::load() const
	{
		std::ifstream fin = std::ifstream(m_path, std::ios::binary);
		if (!fin.is_open())
		{
			V_LOG_WARN(Engine, "load resource from {} failed.", m_path);
			return nullptr;
		}

		if (!VerifyHeaderInfo(fin))
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", m_path);
			return nullptr;
		}

		int32 typeId = 0;
		fin.read((char*)&typeId, sizeof(int32));

		Asset asset;
		if (!LoadAssetMeta(fin, asset))
		{
			V_LOG_WARN(Engine, "load asset from {} failed, , file corrupted. ", m_path);
			return nullptr;
		}

		std::vector<uint8> data;
		if (!loadContentData(fin, data))
		{
			V_LOG_WARN(Engine, "load asset from {} failed, , file corrupted. ", m_path);
			return nullptr;
		}

		std::shared_ptr<ResourceObject> object;
		switch (typeId)
		{
		case Asset::MATERIAL:
			object = std::make_shared<Material>();
			break;
		case Asset::TEXTURE:
			object = std::make_shared<Texture2D>();
			break;
		case Asset::STATIC_MESH:
			object = std::make_shared<StaticMesh>();
			break;
		default:
			break;
		}

		if (!object)
		{
			V_LOG_WARN(Engine, "load asset from {} failed, , file type {} not supported. ", m_path, typeId);
			return nullptr;
		}

		Serializer serializer;
		serializer.setData(std::move(data));
		object->setAsset(asset);
		object->deserialize(serializer);
		return object;
	}

	Asset AssetReader::loadAsset() const
	{
		std::ifstream fin = std::ifstream(m_path, std::ios::binary);
		if (!fin.is_open())
		{
			V_LOG_WARN(Engine, "load resource from {} failed.", m_path);
			return {};
		}

		if (!VerifyHeaderInfo(fin))
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", m_path);
			return {};
		}

		Asset asset;
		if (!LoadAssetMeta(fin, asset))
		{
			V_LOG_WARN(Engine, "load asset from {} failed, , file corrupted. ", m_path);
			return {};
		}

		return asset;
	}
}