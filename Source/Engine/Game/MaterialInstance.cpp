#include <Game/MaterialInstance.h>
#include <Render/Renderer.h>
#include <Render/MaterialProxy.h>

namespace volucris
{
	MaterialInstance::MaterialInstance()
		: GameObject()
		, m_floatParameters()
		, m_vec4Parameters()
		, m_proxy()
		, m_dirty(false)
	{
	}

	MaterialInstance::MaterialInstance(std::string vss, std::string fss)
		: GameObject()
		, m_floatParameters()
		, m_vec4Parameters()
		, m_proxy()
		, m_dirty(false)
	{
	}

	void MaterialInstance::setMaterial(const SoftObject<Material>& material)
	{
		m_material = material;
		if (!m_material)
		{
			m_material.tryLoad();
		}

		if (m_material)
		{

		}
	}

	std::vector<MaterialParameterInfo> MaterialInstance::getParameters()
	{
		std::vector<MaterialParameterInfo> parameters;
		parameters.reserve(m_floatParameters.size() + m_vec4Parameters.size());
		for (auto& parameter : m_floatParameters)
		{
			parameter.setId(parameters.size());
			parameters.push_back(parameter.getParameterInfo());
		}

		for (auto& parameter : m_vec4Parameters)
		{
			parameter.setId(parameters.size());
			parameters.push_back(parameter.getParameterInfo());
		}
		return parameters;
	}

	MaterialFloatParameter& MaterialInstance::addParameter(const std::string& name, float value)
	{
		m_floatParameters.push_back({ name, value });
		return *m_floatParameters.rbegin();
	}

	MaterialVector4Parameter& MaterialInstance::addParameter(const std::string& name, glm::vec4 value)
	{
		m_vec4Parameters.push_back({ name, value });
		return *m_vec4Parameters.rbegin();
	}

	std::vector<MaterialParameterUpdateInfo> MaterialInstance::getUpdateParameterInfos()
	{
		std::vector<MaterialParameterUpdateInfo> parameters;
		for (auto& parameter : m_floatParameters)
		{
			if (parameter.isDirty())
			{
				parameters.push_back(parameter.getUpdateInfo());
				parameter.markDirty(false);
			}
		}

		for (auto& parameter : m_vec4Parameters)
		{
			if (parameter.isDirty())
			{
				parameters.push_back(parameter.getUpdateInfo());
				parameter.markDirty(false);
			}
		}
		return parameters;
	}

	bool MaterialInstance::setFloatParameter(const std::string& name, float value)
	{
		for (auto& param : m_floatParameters)
		{
			if (param.getName() == name)
			{
				param.setValue(value);
				m_dirty = true;
				return true;
			}
		}
		return false;
	}

	bool MaterialInstance::setVector4Parameter(const std::string& name, const glm::vec4& value)
	{
		for (auto& param : m_vec4Parameters)
		{
			if (param.getName() == name)
			{
				param.setValue(value);
				m_dirty = true;
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<MaterialProxy> MaterialInstance::getProxy()
	{
		std::shared_ptr<MaterialProxy> proxy = nullptr;
		if (!m_proxy.expired())
		{
			proxy = m_proxy.lock();
		}

		if (!proxy)
		{
			proxy = std::make_shared<MaterialProxy>();
			
			Renderer::getInstance().push([proxy, parameters = getParameters(), vss=m_vss, fss=m_fss]() {
				proxy->setSource(vss, fss);
				proxy->setParameters(parameters);
				});
			m_proxy = proxy;
			m_dirty = false;
		}
		return proxy;
	}

	void MaterialInstance::update()
	{
		if (!m_dirty)
		{
			return;
		}

		auto proxy = getProxy();
		Renderer::getInstance().push([proxy, parameters = getUpdateParameterInfos()]() {
			proxy->update(parameters);
			});
		m_dirty = false;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::MaterialInstance)