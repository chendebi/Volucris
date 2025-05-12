#include "Resource/material.h"
#include "Core/volucris.h"

namespace volucris
{
	MaterialResource::MaterialResource()
		: ResourceObject()
		, m_vss()
		, m_fss()
		, m_proxy(nullptr)
	{
	}

	MaterialResource::MaterialResource(const std::string& vss, const std::string& fss)
		: MaterialResource()
	{
		setSource(vss, fss);
	}

	void MaterialResource::setSource(const std::string& vss, const std::string& fss)
	{
		m_vss = vss;
		m_fss = fss;
	}

	Material::Material(const std::shared_ptr<MaterialResource>& resource)
		: RenderableObject()
		, m_resource(resource)
		, m_parameters()
	{
		for(const auto& [type, names] : resource->m_parameters)
		{
			for (const auto& name : names)
			{
				if (auto parameter = MaterialParameter::create(type, name))
				{
					m_parameters[name] = std::move(parameter);
				}
				else
				{
					V_LOG_WARN(Engine, "find invalid parameter[{}], type: {}", name, (int)type);
				}
			}
		}
	}

}
