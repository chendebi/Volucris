#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;
	class SectionRenderData;
	class MaterialProxy;
	class MeshProxy;

	class MeshRenderInfo;

	struct PrimitiveInfo
	{
		glm::mat4 localToWorldMatrix;
	};

	struct SectionDrawData
	{
		MeshRenderInfo* renderInfo = nullptr;
		SectionRenderData* section = nullptr;
	};

	struct PrimitiveDrawBatch
	{
		MaterialProxy* material = nullptr;
		PrimitiveInfo* primitiveInfo = nullptr;
		std::vector<SectionDrawData> sections = {};
	};

	class PrimitiveProxy
	{
	public:
		PrimitiveProxy();

		void initialize(PrimitiveComponent* Primitive);

		void setPrimitiveInfo(const PrimitiveInfo& info) { m_primitiveInfo = info; }

		const std::vector<PrimitiveDrawBatch>& getDrawBatch() const { return m_batches; }
		
		void update();

	private:
		
		std::shared_ptr<MeshProxy> m_meshProxy;
		std::vector<PrimitiveDrawBatch> m_batches;
		PrimitiveInfo m_primitiveInfo;
		std::unordered_map<std::shared_ptr<MaterialProxy>, std::vector<SectionRenderData>> m_sections;
	};
}

#endif // !__volucris_primitive_proxy_h__
