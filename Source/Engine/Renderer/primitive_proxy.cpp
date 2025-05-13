#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Resource/mesh_resource_data.h"
#include "Core/assert.h"
#include "Renderer/material_proxy.h"
#include "Resource/material.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_vertex_array_object.h"

namespace volucris
{
	PrimitiveRenderInfo::PrimitiveRenderInfo()
		: vbo(nullptr), ebo(nullptr), vao(nullptr)
	{
		vbo = std::make_shared<OGLBufferObject>(GL_ARRAY_BUFFER);
		ebo = std::make_shared<OGLBufferObject>(GL_ELEMENT_ARRAY_BUFFER);
		vao = std::make_shared<OGLVertexArrayObject>();
	}

	PrimitiveProxy::PrimitiveProxy(PrimitiveComponent* primitive)
		: m_renderData(primitive->getMeshResourceData()->build())
	{
		auto meshData = primitive->getMeshResourceData();
		std::vector<SectionRenderData> sections;
		for (const auto& section : meshData->getSections())
		{
			SectionRenderData sectionRenderData;
			sectionRenderData.mode = section.mode;
			sectionRenderData.count = section.count;
			sectionRenderData.offset = section.offset;
			auto mat = meshData->getMaterial(section.slot);

			check(mat != nullptr);
			sectionRenderData.material = mat->attachProxy();
			sections.push_back(sectionRenderData);
		}
		m_renderData->sections = sections;

		m_renderInfo.vbo->setData(m_renderData->renderData.data(), m_renderData->renderData.size());
		m_renderInfo.ebo->setData(m_renderData->sectionData.data(), m_renderData->sectionData.size());
		m_renderInfo.vao->setVertexBufferObject(m_renderInfo.vbo);

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
				desc.offset = 0;
				desc.stride = block;
			}
			default:
				break;
			}
		}

		m_renderInfo.vao->setDescriptions();
	}
}
