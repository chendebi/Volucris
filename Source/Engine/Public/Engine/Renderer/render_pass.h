#ifndef __volucris_render_pass_h__
#define __volucris_render_pass_h__

#include <memory>
#include "Engine/Renderer/primitive_proxy.h"
#include <unordered_map>

namespace volucris
{
	class SceneProxy;
	class Context;
	class ViewProxy;
	class Texture2DObject;
	class RenderTargetProxy;

	class RenderPass
	{
	public:
		RenderPass() = default;

		virtual ~RenderPass() = default;

		virtual void initialize(ViewProxy* viewport) {}

		void addBatch(const PrimitiveDrawBatch& batch)
		{
			if (shouldCollectBatch(batch))
			{
				for (auto& renderBatch : m_renderBatches)
				{
					if (renderBatch.join(batch))
					{
						return;
					}
				}
				m_renderBatches.push_back(batch);
			}
		}

		void clear()
		{
			m_renderBatches.clear();
		}

		void setRenderTarget(const std::shared_ptr<RenderTargetProxy>& renderTarget)
		{
			m_renderTarget = renderTarget;
		}

		virtual void render(Context* context) {}

		virtual void viewSizeChanged(int width, int height) {}

		virtual std::shared_ptr<RenderPass> clone() = 0;

		virtual std::shared_ptr<RenderTargetProxy> getRenderTarget() const { return m_renderTarget; }

	protected:
		virtual bool shouldCollectBatch(const PrimitiveDrawBatch& batch) const { return false; }

	protected:
		std::vector<PrimitiveDrawBatch> m_renderBatches;
		std::shared_ptr<RenderTargetProxy> m_renderTarget;
	};
}

#endif // !__volucris_render_pass_h__
