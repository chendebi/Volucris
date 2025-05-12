#ifndef __volucris_proxy_object_h__
#define __volucris_proxy_object_h__

namespace volucris
{
	class RenderableObject;

	class ProxyObject
	{
	public:
		virtual ~ProxyObject();

		void setSceneObject(RenderableObject* object)
		{
			m_sceneObject = object;
		}

	private:
		RenderableObject* m_sceneObject;
	};
}

#endif // !__volucris_proxy_object_h__
