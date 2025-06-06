#include "Resource/resource_object.h"
#include "Resource/resource_registry.h"
#include <fmt/format.h>
#include <Core/volucris.h>

namespace volucris
{
    ResourceObject::ResourceObject(AssetType type)
        : m_dirty(true)
        , m_asset(type)
    {
    }

    void ResourceObject::setAsset(const Asset& asset)
    {
        if (asset.type != m_asset.type)
        {
            V_LOG_ERROR(Engine, "try set invalid asset info to asset object. type {}, object type {}", (int)asset.type, (int)m_asset.type);
        }
        else
        {
            m_asset = asset;
        }
    }
}
