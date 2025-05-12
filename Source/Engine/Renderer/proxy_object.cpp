#include "Renderer/proxy_object.h"
#include "Scene/renderable_object.h"
#include "Scene/scene.h"
#include "Application/application.h"

namespace volucris
{
	ProxyObject::~ProxyObject()
	{
		if (m_sceneObject)
		{
			auto object = m_sceneObject;
			gApp->pushCommand([object]() {
				object->setProxy(nullptr);
				});
		}
	}
}
