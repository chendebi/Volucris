#include "Resource/resource_object.h"
#include "Resource/resource_registry.h"
#include <fmt/format.h>

namespace volucris
{
    void ResourceObject::setResourceName(const std::string& name)
    {
        m_path.name = name;
        if (m_metaData.isValid())
        {
            const auto fullpath = fmt::format("{}/{}", m_path.path, name);
            ResourceRegistry::Instance().updateResourcePath(this, fullpath);
        }
    }
}
