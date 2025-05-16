#ifndef __volucris_primitive_component_h__
#define __volucris_primitive_component_h__

#include "Engine/Scene/scene_component.h"
#include "Engine/Renderer/mesh_render_data.h"

namespace volucris
{
	class PrimitiveProxy;
	class MaterialParameter;

	class PrimitiveComponent : public SceneComponent
	{
	public:
		PrimitiveComponent();

		void setResourceData(const std::shared_ptr<MeshResourceData>& data);

		MeshResourceData* getMeshResourceData();

	protected:
		void updateRenderState() override;

		void onTransformChanged() override;

		void updateTransform() override;

	private:
		PrimitiveProxy* m_proxy;
		std::shared_ptr<MeshResourceData> m_meshData;
		std::vector<MaterialParameter*> m_modelMatParameters;
	};
}

#endif // !__volucris_primitive_component_h__
