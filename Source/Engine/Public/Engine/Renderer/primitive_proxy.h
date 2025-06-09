#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>
#include <vector>
#include <unordered_map>

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;
	class SectionRenderData;
	class MaterialProxy;
	class MeshProxy;
	class UniformValueMat4;

	class MeshRenderInfo;


	struct SectionDrawData
	{
		MeshRenderInfo* renderInfo = nullptr;
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
		PrimitiveProxy();

		void initialize(PrimitiveComponent* Primitive);

		std::vector<PrimitiveDrawBatch> getDrawBatch() const { return m_batches; }

	private:
		std::shared_ptr<MeshProxy> m_meshProxy;
		std::vector<PrimitiveDrawBatch> m_batches;
		std::unordered_map<std::shared_ptr<MaterialProxy>, std::vector<SectionRenderData>> m_sections;
	};
}

#endif // !__volucris_primitive_proxy_h__
