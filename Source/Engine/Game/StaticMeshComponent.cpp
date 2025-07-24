#include "Game/StaticMeshComponent.h"
#include <Core/Volucris.h>
#include <Render/PrimitiveSceneProxy.h>

namespace volucris
{
	StaticMeshComponent::StaticMeshComponent()
		: SceneComponent()
		, m_mesh()
		, m_proxy()
	{
	}

	StaticMeshComponent::StaticMeshComponent(const SoftObject<StaticMesh>& mesh)
		: StaticMeshComponent()
	{
		setStaticMesh(mesh);
	}

	void StaticMeshComponent::setStaticMesh(const SoftObject<StaticMesh>& mesh)
	{
		m_mesh = mesh;
		if (!m_mesh.object() && !m_mesh.tryLoad())
		{
			V_LOG_WARN(Engine, "StaticMeshComponent accept invalid mesh");
		}
		markRenderStateDirty();
	}

	std::shared_ptr<PrimitiveSceneProxy> StaticMeshComponent::createProxy()
	{
		auto proxy = std::make_shared<PrimitiveSceneProxy>();
		m_proxy = proxy;
		return proxy;
	}

	void StaticMeshComponent::onRenderStateChanged()
	{
		std::shared_ptr<PrimitiveSceneProxy> proxy = nullptr;
		if (!m_proxy.expired())
		{
			proxy = m_proxy.lock();
		}

		auto mesh = m_mesh.object();
		if (m_mesh.object() && !proxy)
		{

		}
	}

}
