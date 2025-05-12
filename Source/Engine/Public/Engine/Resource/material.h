#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include "Engine/Resource/resource_object.h"
#include <memory>
#include <vector>
#include <map>
#include "material_parameter.h"
#include "Engine/Scene/renderable_object.h"

namespace volucris
{
	class MaterialProxy;

	class MaterialResource : public ResourceObject, public RenderableObject
	{
	public:
		using MaterialParameterMap = std::unordered_map<MaterialParameter::Type, std::vector<std::string>>;

		MaterialResource();

		MaterialResource(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		void setParameters(const MaterialParameterMap& parameters)
		{
			m_parameters = parameters;
		}

		std::string getVertexShaderSource() const { return m_vss; }
		std::string getFragmentShaderSource() const { return m_fss; }

	private:
		friend class Material;
		std::string m_vss;
		std::string m_fss;
		MaterialParameterMap m_parameters;
	};

	class Material : public RenderableObject
	{
	public:
		Material(const std::shared_ptr<MaterialResource>& resource);

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

	private:
		std::shared_ptr<MaterialResource> m_resource;
		std::unordered_map<std::string, std::unique_ptr<MaterialParameter>> m_parameters;
	};
}

#endif // !__volucris_material_h__
