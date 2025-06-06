#include "Resource/resource_object.h"
#include "Resource/resource_registry.h"
#include <fmt/format.h>
#include <Core/volucris.h>

namespace volucris
{
    ResourceObject::ResourceObject(Asset::Type type)
        : m_dirty(true)
        , m_type(type)
        , m_asset()
    {
    }

    void ResourceObject::setAsset(const Asset& asset)
    {
        m_asset = asset;
    }
}
