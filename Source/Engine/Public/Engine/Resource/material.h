#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <Engine/Resource/resource_object.h>

namespace volucris
{
	class MaterialResource;
	class MaterialProxy;
	class MaterialParameter;
	class MaterialParameterTexture2D;

	class Material : public ResourceObject
	{
	public:
		Material();

		Material(const std::shared_ptr<MaterialResource>& resource);

		Material(const std::shared_ptr<Material>& parent);

		~Material();

		std::shared_ptr<MaterialProxy> getRenderProxy();

		void update();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

		MaterialParameter* getParameterByName(const std::string& name);

		const std::vector<std::shared_ptr<MaterialParameter>>& getParameters() const { return m_parameters; }

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	protected:
		void setMaterialResource(const std::shared_ptr<MaterialResource>& resource);

		void onSourceRebuild(MaterialResource* resource);

	private:
		void updateParameterData(void* data, size_t offset, size_t size);

	private:
		std::shared_ptr<Material> m_parent;
		std::shared_ptr<MaterialResource> m_resource;
		std::vector<std::shared_ptr<MaterialParameter>> m_parameters;
		std::vector<std::shared_ptr<MaterialParameterTexture2D>> m_textureParameters;
		std::weak_ptr<MaterialProxy> m_proxy;
		std::vector<uint8> m_bufferData;
	};
}

#endif // !__volucris_material_h__
