#include "Resource/material_resource.h"
#include "Application/application.h"
#include "Renderer/material_proxy.h"
#include "Renderer/renderer.h"
#include "Core/volucris.h"

namespace volucris
{
	MaterialResource::MaterialResource()
		: ResourceObject()
		, m_vss()
		, m_fss()
		, m_proxy()
	{
	}

	MaterialResource::~MaterialResource()
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

	std::shared_ptr<MaterialResourceProxy> MaterialResource::getRenderProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<MaterialResourceProxy>(this);
			m_proxy = proxy;
			V_LOG_INFO(Engine, "create material resource proxy: {}");
		}
		return proxy;
	}

	bool MaterialResource::serialize(Serializer& serializer) const
	{
		serializer.serialize(m_vss);
		serializer.serialize(m_fss);
		serializer.serialize(m_descriptions);
		return true;
	}

	void  MaterialResource::deserialize(Serializer& serializer)
	{
		std::string vss, fss;
		std::vector<MaterialParameterDesc> parameters;
		if (!serializer.deserialize(vss) || !serializer.deserialize(fss) || !serializer.deserialize(parameters))
		{
			V_LOG_WARN(Engine, "deserialize material resource failed.");
			return;
		}

		m_vss = std::move(vss);
		m_fss = std::move(fss);
		m_descriptions = std::move(parameters);

		size_t offset = 0;
		for (auto& desc : m_descriptions)
		{
			if (desc.type == MaterialParameterDesc::UNKNOWN)
			{
				continue;
			}

			auto typeSize = MaterialParameterDesc::sizeOfType(desc.type);
			desc.offset = offset;
			offset += typeSize;
		}
	}
}