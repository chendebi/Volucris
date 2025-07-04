#ifndef __volucris_asset_reader_h__
#define __volucris_asset_reader_h__

#include <string>
#include <memory>
#include "Engine/Asset/AssetData.h"

namespace volucris
{
	class Package;
	// 资源读取器接口
	class AssetReader
	{
	public:
		// 构造函数
		AssetReader() = default;

		AssetReader(const std::string& path)
			: m_path(path)
		{

		}

		// 析构函数
		~AssetReader() = default;

		// 获取资源包
		std::shared_ptr<Package> readPackage() const;

		AssetData readAssetData() const;


	private:
		std::string m_path; // 资源包路径
	};
}

#endif // !__volucris_asset_reader_h__
