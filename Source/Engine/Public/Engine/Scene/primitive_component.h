#ifndef __volucris_primitive_component_h__
#define __volucris_primitive_component_h__

#include "Engine/Scene/scene_component.h"
#include "Engine/Renderer/mesh_render_data.h"
#include "Engine/Scene/renderable_object.h"

namespace volucris
{
	class PrimitiveProxy;

	class PrimitiveComponent : public SceneComponent, public RenderableObject
	{
	public:
		PrimitiveComponent();

		void setResourceData(const std::shared_ptr<MeshResourceData>& data);

		MeshResourceData* getMeshResourceData();

	protected:
		void updateRenderState() override;

	private:
		PrimitiveProxy* m_proxy;
		std::shared_ptr<MeshResourceData> m_meshData;
	};
}

#endif // !__volucris_primitive_component_h__
