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
		, m_descriptions()
		, m_bufferSize(0)
		, m_proxy()
	{
	}

	MaterialResource::MaterialResource(const std::string& vss, const std::string& fss)
		: MaterialResource()
	{
		setSource(vss, fss);
	}

	MaterialResource::~MaterialResource()
	{
	}

	void MaterialResource::setSource(const std::string& vss, const std::string& fss)
	{
		m_vss = vss;
		m_fss = fss;
	}

	void MaterialResource::addParameter(const std::string& name, MaterialParameterType type)
	{
		MaterialParameterDescription description;
		description.name = name;
		description.type = type;
		description.offset = m_bufferSize;
		switch (type)
		{
		case volucris::MaterialParameterType::NONE:
			check(false);
			break;
		case volucris::MaterialParameterType::FLOAT:
			description.size = sizeof(float);
			break;
		case volucris::MaterialParameterType::VEC2:
			description.size = sizeof(glm::vec2);
			break;
		case volucris::MaterialParameterType::VEC3:
			description.size = sizeof(glm::vec3);
			break;
		case volucris::MaterialParameterType::VEC4:
			description.size = sizeof(glm::vec4);
			break;
		case volucris::MaterialParameterType::TEXTURE2D:
			description.size = sizeof(int32);
			break;
		default:
			break;
		}
		m_bufferSize += description.size;
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
				renderData.descriptions = m_descriptions;
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
			renderData.descriptions = m_descriptions;
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
		serializer.serialize(m_descriptions);
		return true;
	}

	void  MaterialResource::deserialize(Serializer& serializer)
	{
		std::string vss, fss;
		MaterialUniformBlocks engineDatas;
		std::vector<MaterialParameterDescription> descriptions;
		if (!serializer.deserialize(vss) || !serializer.deserialize(fss) || !serializer.deserialize(engineDatas) ||
			!serializer.deserialize(descriptions))
		{
			V_LOG_WARN(Engine, "deserialize material resource failed.");
			return;
		}

		m_vss = std::move(vss);
		m_fss = std::move(fss);
		m_descriptions = std::move(descriptions);
	}
}