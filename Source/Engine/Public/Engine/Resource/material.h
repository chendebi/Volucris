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

		void update();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

		MaterialParameter* getParameterByName(const std::string& name);

		MaterialParameter* getParameterByType(MaterialParameterDesc::Type type);

		void setMaterialResource(const std::shared_ptr<MaterialResource>& resource);

		const std::string& getVertexShaderFilePath() const
		{
			if (m_parent)
			{
				return m_parent->getVertexShaderFilePath();
			}
			return m_vsFilePath;
		}

		const std::string& getFragmentShaderFilePath() const
		{
			if (m_parent)
			{
				return m_parent->getFragmentShaderFilePath();
			}
			return m_fsFilePath;
		}

	protected:

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	private:
		std::shared_ptr<Material> m_parent;
		std::string m_vsFilePath;
		std::string m_fsFilePath;
		std::shared_ptr<MaterialResource> m_resource;
		std::vector<std::unique_ptr<MaterialParameter>> m_parameters;
		std::weak_ptr<MaterialProxy> m_proxy;
	};
}

#endif // !__volucris_material_h__
