#include <Resource/asset_writer.h>
#include <Resource/resource_object.h>
#include <Resource/asset_reader.h>
#include <Core/volucris.h>
#include <filesystem>
#include <Resource/resource_registry.h>
#include <Resource/asset_header.h>

namespace fs = std::filesystem;

namespace volucris
{
	bool AssetWriter::save(const std::shared_ptr<ResourceObject>& object)
	{
		const auto& asset = object->getAsset();
		if (!asset.uuid.valid())
		{
			V_LOG_WARN(Engine, "save asset failed. asset not registered");
			return false;
		}

		auto assetPath = asset.getAssetPath();
		std::string sysPath;
		if (!ResourceRegistry::Instance().getSystemPathByResourcePath(assetPath, sysPath))
		{
			V_LOG_WARN(Engine, "save asset failed. asset path invalid, {}", assetPath);
			return false;
		}

		bool writable = true;
		if (fs::exists(sysPath))
		{
			AssetReader reader;
			auto existed = reader.loadAsset();
			if (!existed.uuid.valid() || existed.uuid != asset.uuid)
			{
				writable = false;
			}
		}

		if (!writable)
		{
			V_LOG_WARN(Engine, "save asset failed. asset path already have other asset, {}", assetPath);
			return false;
		}

		Serializer resourceSerializer;
		if (!object->serialize(resourceSerializer))
		{
			return false;
		}

		std::ofstream fout = std::ofstream(sysPath, std::ios::binary | std::ios::trunc);
		if (!fout.is_open())
		{
			V_LOG_WARN(Engine, "save resource to {} failed.", assetPath);
			return false;
		}
		fout.write((char*)&AssetHeader::VerifyHeader, sizeof(AssetHeader::VerifyHeader));

		{
			Serializer metaDataSerializer;
			metaDataSerializer.serialize(asset);
			uint32 size = metaDataSerializer.getData().size();
			fout.write((char*)&size, sizeof(uint32));
			fout.write((char*)metaDataSerializer.getData().data(), metaDataSerializer.getData().size());
		}

		uint32 size = resourceSerializer.getData().size();
		fout.write((char*)&size, sizeof(uint32));
		fout.write((char*)resourceSerializer.getData().data(), resourceSerializer.getData().size());

		fout.close();
	}
}