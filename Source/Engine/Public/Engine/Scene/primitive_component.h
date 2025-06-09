#ifndef __volucris_primitive_component_h__
#define __volucris_primitive_component_h__

#include "Engine/Scene/scene_component.h"
#include "Engine/Renderer/mesh_render_data.h"

namespace volucris
{
	class Material;
	class MeshResource;
	class PrimitiveProxy;
	class MaterialValueParameter;

	class PrimitiveComponent : public SceneComponent
	{
	public:
		PrimitiveComponent();

		void setMeshResource(const std::shared_ptr<MeshResource>& resource);

		bool setMaterial(const std::string& slot, const std::shared_ptr<Material>& mat);

		void setMaterials(const std::unordered_map <std::string, std::shared_ptr<Material>>& mats);

		MeshResource* getResource() const { return m_resource.get(); }

		Material* getMaterial(const std::string& mat) const;

	protected:
		void updateRenderState() override;

		void onTransformChanged() override;

		void updateTransform() override;

	private:
		std::weak_ptr<PrimitiveProxy> m_proxy;
		std::shared_ptr<MeshResource> m_resource;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}

#endif // !__volucris_primitive_component_h__
