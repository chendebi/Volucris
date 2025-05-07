#ifndef __volucris_primitive_component_h__
#define __volucris_primitive_component_h__

#include "Engine/Scene/scene_component.h"

namespace volucris
{
	class Section
	{

	};

	class PrimitiveProxy;

	class PrimitiveComponent : public SceneComponent
	{
	public:
		PrimitiveComponent();

		void addVertex(float x, float y, float z)
		{
			addVertex({ x, y, z });
		}

		void addVertex(const glm::vec3& vertex)
		{
			m_vertices.push_back(vertex);
			markRenderStateDirty();
		}

		void setVertices(float* data, size_t size);

		void setNormals(float* data, size_t size);

	protected:
		void updateRenderState() override;

	private:
		PrimitiveProxy* m_proxy;
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec4> m_uvs;
	};
}

#endif // !__volucris_primitive_component_h__
