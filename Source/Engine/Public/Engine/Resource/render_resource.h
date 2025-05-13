#ifndef __volucris_render_resource_h__
#define __volucris_render_resource_h__

#include "Engine/Resource/resource_object.h"
#include <atomic>

namespace volucris
{

	class RenderResource : public ResourceObject
	{
	public:
		void addRenderRef() { ++m_renderRef; }

		void removeRenderRef()
		{
			--m_renderRef;
			if (m_renderRef <= 0)
			{
				releaseRenderProxy();
			}
		}

	protected:
		virtual void releaseRenderProxy() {  }

	private:
		size_t m_renderRef = 0;
	};
}

#endif // !__volucris_render_resource_h__
