#ifndef __volucris_renderable_object_h__
#define __volucris_renderable_object_h__

namespace volucris
{
	class ProxyObject;
	class RenderableObject
	{
	public:
		virtual ~RenderableObject() = default;

		void setProxy(ProxyObject* proxy) { m_proxy = proxy; }

		ProxyObject* getProxy() const { return m_proxy; }

	private:
		ProxyObject* m_proxy = nullptr;
	};
}

#endif // !__volucris_renderable_object_h__
