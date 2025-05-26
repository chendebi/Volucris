#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <Engine/Resource/resource_object.h>
#include <Engine/Resource/material_parameter.h>

namespace volucris
{
	class MaterialResource;
	class MaterialProxy;

	class Material : public ResourceObject
	{
	public:
		Material();

		~Material();

		void setShaderPath(const std::string& vs, const std::string& fs);

		std::shared_ptr<MaterialProxy> getRenderProxy();

		void updateParametersToRenderer();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

		std::vector<uint8> getParameterData() const { return  m_parameterData; }

		MaterialParameter* getParameterByName(const std::string& name);

		MaterialParameter* getParameterByType(MaterialParameterDesc::Type type);

		void setMaterialResource(const std::shared_ptr<MaterialResource>& resource);

	protected:

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	private:
		std::shared_ptr<Material> m_parent;
		std::string m_vsFilePath;
		std::string m_fsFilePath;
		std::shared_ptr<MaterialResource> m_resource;
		std::vector<std::unique_ptr<MaterialParameter>> m_parameters;
		std::vector<uint8> m_parameterData;
		std::weak_ptr<MaterialProxy> m_proxy;
	};
}

#endif // !__volucris_material_h__
