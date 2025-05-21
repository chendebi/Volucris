#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "material_parameter.h"
#include "Engine/Resource/render_resource.h"

namespace volucris
{
	class MaterialResource;
	class MaterialProxy;

	class Material : public RenderResource
	{
	public:
		Material();

		~Material();

		MaterialProxy* getProxy() const { return m_proxy; }

		MaterialProxy* attachProxy();

		void deattachProxy();

		void updateParametersToRenderer();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

		std::vector<uint8> getParameterData() const { return  m_parameterData; }

		MaterialParameter* getParameterByName(const std::string& name);

		MaterialParameter* getParameterByType(MaterialParameterDesc::Type type);

	protected:
		void releaseRenderProxy() override;

		bool serialize(rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator) const override;

		void deserialize(const rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator) override;

	private:
		void setMaterialResource(const std::shared_ptr<MaterialResource>& resource);

	private:
		std::shared_ptr<Material> m_parent;
		std::string m_vsFilePath;
		std::string m_fsFilePath;
		std::shared_ptr<MaterialResource> m_resource;
		std::vector<std::unique_ptr<MaterialParameter>> m_parameters;
		std::vector<uint8> m_parameterData;
		MaterialProxy* m_proxy;
	};
}

#endif // !__volucris_material_h__
