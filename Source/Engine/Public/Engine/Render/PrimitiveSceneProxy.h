#ifndef __volucris_primitive_scene_proxy_h__
#define __volucris_primitive_scene_proxy_h__

#include <Engine/Render/StaticMeshProxy.h>

namespace volucris
{
	class MaterialProxy;

	class PrimitiveSceneProxy
	{
	public:
		PrimitiveSceneProxy();

		~PrimitiveSceneProxy();

		void setMesh(const std::shared_ptr<StaticMeshProxy>& mesh)
		{
			m_meshProxy = mesh;
		}

		void setMaterials(const std::vector<std::shared_ptr<MaterialProxy>>& materials)
		{
			m_materials = materials;
		}

	private:
		std::shared_ptr<StaticMeshProxy> m_meshProxy;
		std::vector<std::shared_ptr<MaterialProxy>> m_materials;
		//std::shared_ptr<Mater>
	};
}

#endif // !__volucris_primitive_scene_proxy_h__
