#ifndef __volucris_static_mesh_proxy_h__
#define __volucris_static_mesh_proxy_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/PrimitiveInfo.h>
#include <Engine/RHI/RHIState.h>

namespace volucris
{
	class RHIVertexArray;
	class RHIElementBuffer;

	class StaticMeshProxy : public Object
	{
	public:
		StaticMeshProxy();

		bool init(const PrimitiveInfo& info);

		DrawInfo* get(size_t index)
		{
			return &m_drawInfos[index];
		}

		const std::vector<DrawInfo> getDrawInfos() const { return m_drawInfos; }

	private:
		std::shared_ptr<RHIVertexArray> m_vao;
		std::shared_ptr<RHIElementBuffer> m_ebo;
		std::vector<DrawInfo> m_drawInfos;
	};
}

#endif // !__volucris_static_mesh_proxy_h__
