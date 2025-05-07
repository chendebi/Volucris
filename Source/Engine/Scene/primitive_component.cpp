#include "Scene/primitive_component.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/application.h>
#include <Renderer/renderer.h>

namespace volucris
{
	PrimitiveComponent::PrimitiveComponent()
		: SceneComponent()
		, m_proxy(nullptr)
		, m_vertices()
	{
	}

	void PrimitiveComponent::setVertices(float* data, size_t size)
	{
		auto count = size / sizeof(glm::vec3);
		checkf(count * sizeof(glm::vec3) == size, Engine, "vertex data invalid");
		m_vertices.resize(count);
		memcpy(m_vertices.data(), data, size);
		markRenderStateDirty();
	}

	void PrimitiveComponent::setNormals(float* data, size_t size)
	{
		auto count = size / sizeof(glm::vec3);
		checkf(count * sizeof(glm::vec3) == size, Engine, "vertex data invalid");
		m_normals.resize(count);
		memcpy(m_normals.data(), data, size);
		markRenderStateDirty();
	}

	void PrimitiveComponent::updateRenderState()
	{
		auto renderer = gApp->getRenderer();
		if (!renderer)
		{
			m_proxy = nullptr;
			return;
		}

		if (m_proxy)
		{
			renderer->pushCommand([this]() {
				// 删除
				});
		}

		// 创建


	}

}
