#include "Game/MeshResourceData.h"

namespace volucris
{
	void MeshData::addSubMesh(SmallSubMesh mesh)
	{
		PrimitiveSegment segment;
		segment.offset = m_submeshData.size();
		segment.mode = mesh.drawMode;
		segment.count = mesh.indices.size();
		segment.type = ElementDataType::UByte;
		const auto appendSize = mesh.getIndexDataSize();
		m_submeshData.resize(appendSize);
		memcpy(m_submeshData.data() + segment.offset, mesh.indices.data(), appendSize);
		m_segments.push_back(segment);
	}

	void MeshData::addSubMesh(MediumSubMesh mesh)
	{
		PrimitiveSegment segment;
		segment.offset = m_submeshData.size();
		segment.mode = mesh.drawMode;
		segment.count = mesh.indices.size();
		segment.type = ElementDataType::UShort;
		const auto appendSize = mesh.getIndexDataSize();
		m_submeshData.resize(segment.offset + appendSize);
		memcpy(m_submeshData.data() + segment.offset, mesh.indices.data(), appendSize);
		m_segments.push_back(segment);
	}

	void MeshData::addSubMesh(LargeSubMesh mesh)
	{
		PrimitiveSegment segment;
		segment.offset = m_submeshData.size();
		segment.mode = mesh.drawMode;
		segment.count = mesh.indices.size();
		segment.type = ElementDataType::UInt;
		const auto appendSize = mesh.getIndexDataSize();
		m_submeshData.resize(segment.offset + appendSize);
		memcpy(m_submeshData.data() + segment.offset, mesh.indices.data(), appendSize);
		m_segments.push_back(segment);
	}
}
