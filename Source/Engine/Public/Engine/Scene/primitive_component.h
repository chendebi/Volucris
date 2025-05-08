#ifndef __volucris_primitive_component_h__
#define __volucris_primitive_component_h__

#include "Engine/Scene/scene_component.h"
#include <Engine/Resource/mesh_resource_data.h>

namespace volucris
{
	class PrimitiveProxy;

	class PrimitiveComponent : public SceneComponent
	{
	public:
		PrimitiveComponent();

		MeshResourceData* getMeshResourceData();

	protected:
		void updateRenderState() override;

	private:
		PrimitiveProxy* m_proxy;
		std::unique_ptr<MeshResourceData> m_meshData;
	};
}

#endif // !__volucris_primitive_component_h__
