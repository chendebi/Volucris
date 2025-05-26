#include "Renderer/mesh_proxy.h"
#include "Resource/mesh_resource_data.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_vertex_array_object.h"

namespace volucris
{
	MeshRenderInfo::MeshRenderInfo()
		: vbo(std::make_shared<OGLBufferObject>(GL_ARRAY_BUFFER))
		, ebo(std::make_shared<OGLBufferObject>(GL_ELEMENT_ARRAY_BUFFER))
		, vao(std::make_shared<OGLVertexArrayObject>())
	{
		vao->setVertexBufferObject(vbo);
	}

	MeshProxy::MeshProxy()
		: m_renderData(nullptr)
		, m_renderInfo(std::make_unique<MeshRenderInfo>())
	{

	}

	MeshProxy::MeshProxy(MeshResourceData* resource)
		: MeshProxy()
	{
		updateByResource(resource);
	}

	void MeshProxy::updateByResource(MeshResourceData* resource)
	{
		m_renderData = resource->build();
		m_renderInfo->vbo->setData(m_renderData->renderData.data(), m_renderData->renderData.size());
		m_renderInfo->ebo->setData(m_renderData->sectionData.data(), m_renderData->sectionData.size());

		for (const auto& block : m_renderData->blocks)
		{
			BufferDescription desc;
			switch (block.type)
			{
			case BlockType::VERTEX:
			{
				desc.type = GL_FLOAT;
				desc.normalized = false;
				desc.size = 3;
				desc.offset = block.offset;
				desc.stride = 3 * sizeof(float);
			}
			break;
			default:
				break;
			}
			m_renderInfo->vao->addDescription(desc);
		}
	}
}

