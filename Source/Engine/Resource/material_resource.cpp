#include "Resource/material_resource.h"
#include "Application/application.h"
#include "Renderer/material_proxy.h"
#include "Renderer/renderer.h"
#include "Core/volucris.h"

namespace volucris
{
	MaterialResource::MaterialResource()
		: m_dirty(false)
		, m_vss()
		, m_fss()
		, m_innerParameters(0)
		, m_descriptions()
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

	void MaterialResource::setParameterDescriptions(const std::vector<MaterialParameterDescription>& descriptions)
	{
		m_descriptions = descriptions;
	}

	void MaterialResource::dirty()
	{
		m_dirty = true;
#if WITH_EDITOR
		Rebuild(this);
#endif // WITH_EDITOR
	}

	void MaterialResource::update()
	{
		if (m_dirty)
		{
			if (auto proxy = m_proxy.lock())
			{
				MaterialRenderData renderData;
				renderData.vss = m_vss;
				renderData.fss = m_fss;
				for (const auto& desc : m_descriptions)
				{
					renderData.parameterNames.push_back(desc.name);
				}
				renderData.engineParameters = m_innerParameters;
				gApp->getRenderer()->pushCommand([proxy, renderData]() {
					proxy->update(renderData);
					});
			}
			m_dirty = false;
		}
	}

	std::shared_ptr<MaterialResourceProxy> MaterialResource::getRenderProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			MaterialRenderData renderData;
			renderData.vss = m_vss;
			renderData.fss = m_fss;
			for (const auto& desc : m_descriptions)
			{
				renderData.parameterNames.push_back(desc.name);
			}
			renderData.engineParameters = m_innerParameters;
			proxy = std::make_shared<MaterialResourceProxy>();
			proxy->update(std::move(renderData));
			m_proxy = proxy;
		}
		return proxy;
	}

	bool MaterialResource::serialize(Serializer& serializer) const
	{
		serializer.serialize(m_vss);
		serializer.serialize(m_fss);
		serializer.serialize(m_innerParameters);
		return true;
	}

	void  MaterialResource::deserialize(Serializer& serializer)
	{
		std::string vss, fss;
		MaterialInnerParameters engineDatas;
		if (!serializer.deserialize(vss) || !serializer.deserialize(fss) || !serializer.deserialize(engineDatas))
		{
			V_LOG_WARN(Engine, "deserialize material resource failed.");
			return;
		}

		m_vss = std::move(vss);
		m_fss = std::move(fss);
		m_innerParameters = engineDatas;
	}
}