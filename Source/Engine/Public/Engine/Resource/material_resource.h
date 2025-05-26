#ifndef __volucris_material_resource_h__
#define __volucris_material_resource_h__

#include <Engine/Resource/resource_object.h>
#include "Engine/Resource/material_parameter.h"

namespace volucris
{
	class MaterialResourceProxy;

	class MaterialResource : public ResourceObject
	{
	public:
		MaterialResource();

		~MaterialResource();

		MaterialResource(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		void setParameters(const std::vector<MaterialParameterDesc>& parameters)
		{
			m_descriptions = parameters;
		}

		std::shared_ptr<MaterialResourceProxy> getRenderProxy();

		std::string getVertexShaderSource() const { return m_vss; }

		std::string getFragmentShaderSource() const { return m_fss; }

		const std::vector<MaterialParameterDesc>& getParameterDescriptions() const { return m_descriptions; }

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	private:
		std::string m_vss;
		std::string m_fss;
		std::vector<MaterialParameterDesc> m_descriptions;
		std::weak_ptr<MaterialResourceProxy> m_proxy;
	};

	class MaterialResourceLoader
	{
	public:
		MaterialResourceLoader() = default;

		std::shared_ptr<MaterialResource> loadMaterialResource(const std::string& vsf, const std::string& fsf);
	};
}

#endif // !__volucris_material_resource_h__
