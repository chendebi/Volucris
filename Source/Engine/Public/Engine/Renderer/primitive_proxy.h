#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>
#include <vector>

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;
	class SectionRenderData;
	class MaterialProxy;

	class OGLBufferObject;
	class OGLVertexArrayObject;

	struct PrimitiveRenderInfo
	{
		std::shared_ptr<OGLBufferObject> vbo;
		std::shared_ptr<OGLBufferObject> ebo;
		std::shared_ptr<OGLVertexArrayObject> vao;

		PrimitiveRenderInfo();
	};

	struct SectionDrawData
	{
		PrimitiveRenderInfo* renderInfo = nullptr;
		SectionRenderData* section = nullptr;
	};

	struct PrimitiveDrawBatch
	{
		MaterialProxy* material = nullptr;
		std::vector<SectionDrawData> sections = {};

		bool join(const PrimitiveDrawBatch& other)
		{
			if (other.material == material)
			{
				sections.reserve(sections.size() + other.sections.size());
				for (const auto& section : other.sections)
				{
					sections.push_back(section);
				}
				return true;
			}
			return false;
		}
	};

	class PrimitiveProxy
	{
	public:
		PrimitiveProxy(PrimitiveComponent* Primitive);

		PrimitiveRenderInfo getRenderInfo() const { return m_renderInfo; }

		std::vector<PrimitiveDrawBatch> getDrawBatch() const { return m_batches; }

	private:
		std::shared_ptr<MeshRenderData> m_renderData;
		PrimitiveRenderInfo m_renderInfo;
		std::vector<PrimitiveDrawBatch> m_batches;
	};
}

#endif // !__volucris_primitive_proxy_h__
